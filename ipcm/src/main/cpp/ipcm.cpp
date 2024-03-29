//
// Created by nebula on 2019-06-05.
//

#include "ipcm.h"
#include "mmap_file.h"
#include "thread_lock.h"
#include "utils.h"
#include <unordered_map>

using namespace std;
static unordered_map<string, IPCM *> *s_instance_dic;
static ThreadLock s_instance_lock;
static string s_root_dir;


static string md5(const string &value);

static string gen_map_key(const string &map_id, string *relative_path);

//static string

IPCM::IPCM(const string &map_id, int size)
        : m_lock(0),
          m_shared_lock(&m_lock, FileLock::TYPE_SHARED),
          m_exclusive_lock(&m_lock, FileLock::TYPE_EXCLUSIVE),
          m_memory_map_file("") {
    m_fd = -1;
    m_memory_ptr = nullptr;
    m_size = 0;
    m_real_size = 0;
    m_sink_data = nullptr;

    {
        SCOPE_LOCK(m_shared_lock);
        load_file_data();
    }
}

void init_thread() {
    s_instance_dic = new unordered_map<string, IPCM *>;
    s_instance_lock = ThreadLock();
}

void IPCM::init(const std::string &root_dir) {
    static pthread_once_t run_once = PTHREAD_ONCE_INIT;
    pthread_once(&run_once, init_thread);

    s_root_dir = root_dir;
    char *path = strdup(s_root_dir.c_str());
    if (path) {
        mk_path(path);
        free(path);
    }
    LOGD("root dir: %s", s_root_dir.c_str());

}

IPCM *IPCM::default_instance(size_t mode) {
    return create_instance(DEFAULT_MAP_KEY, getpagesize(), mode);
}

IPCM *IPCM::create_instance(const std::string &map_id, int page_size, size_t mode,
                            string *relative_path) {
    if (map_id.empty()) {
        return nullptr;
    }
    SCOPE_LOCK(s_instance_lock);
    auto key = gen_map_key(map_id, nullptr);
    auto iterator = s_instance_dic->find(key);
    if (iterator != s_instance_dic->end()) {
        return iterator->second;
    }
    //todo:deal with relative path
    auto ptr = new IPCM(map_id, 0);
    (*s_instance_dic)[key] = ptr;
    return ptr;
}

bool IPCM::set_memory_data_by_key(const std::string &key, IPCBuffer &&buffer) {
    if (buffer.length() == 0 || key.empty()) {
        return false;
    }
    SCOPE_LOCK(m_thread_lock);
    SCOPE_LOCK(m_exclusive_lock);
    auto iter = m_dic.find(key);
    if (iter == m_dic.end()) {
        iter = m_dic.emplace(key, move(buffer)).first;
    } else {
        iter->second = move(buffer);
    }
    return append_data_by_key(key, iter->second);
}


IPCBuffer &IPCM::get_memory_data_by_key(const std::string &key) {
    auto iter = m_dic.find(key);
    if (iter != m_dic.end()) {
        return iter->second;
    }
    static IPCBuffer zero(0);
    return zero;
}

bool IPCM::append_data_by_key(const std::string &key, const IPCBuffer &buffer) {
    size_t total_size = key.length() + cal_varint32_size(static_cast<int32_t>(key.length()));
    total_size += buffer.length() + cal_varint32_size(static_cast<int32_t>(buffer.length()));
    if (m_real_size == 0) {
        //encode all dic to buffer
        auto buff = ProtoBuffCoder::encode_data_to_buff(m_dic);
        if (buff.length() > 0) {

            write_real_size(buff.length());
            m_sink_data->write_raw_buff(buff, 0);
            return true;
        }
        return false;
    } else {
        write_real_size(m_real_size + total_size);
        m_sink_data->write_string(key);
        m_sink_data->write_buff(buffer);
        return true;
    }
}

bool IPCM::encode_int32(const std::string &key, int32_t value) {
    if (key.empty()) {
        return false;
    }
    size_t size = cal_varint32_size(value);
    IPCBuffer buffer(size);
    SinkData data(buffer.get_ptr(), size);
    data.write_int32(value);
    return set_memory_data_by_key(key, move(buffer));
}

bool IPCM::encode_int64(const std::string &key, int64_t value) {
    if (key.empty()) {
        return false;
    }
    size_t size = cal_varint64_size(value);
    IPCBuffer buffer(size);
    SinkData data(buffer.get_ptr(), size);
    data.write_int64(value);
    return set_memory_data_by_key(key, move(buffer));
}


int32_t IPCM::decode_int32(const std::string &key, int32_t default_value) {
    if (key.empty()) {
        return default_value;
    }
    auto &data = get_memory_data_by_key(key);
    if (data.length() > 0) {
        SourceData source(data.get_ptr(), data.length());
        return source.read_int32();
    }
    return default_value;
}

int64_t IPCM::decode_int64(const std::string &key, int64_t default_value) {
    if (key.empty()) {
        return default_value;
    }
    auto &data = get_memory_data_by_key(key);
    if (data.length() > 0) {
        SourceData source(data.get_ptr(), data.length());
        return source.read_int64();
    }
    return default_value;
}

void IPCM::write_real_size(size_t real_size) {
    memcpy(m_memory_ptr, &real_size, 4);
    m_real_size = real_size;
}

void IPCM::load_file_data() {
    //read meta info
    if (m_memory_map_file.is_file_valid()) {
        m_meta_info.read(m_memory_map_file.get_ptr());
    }
    m_fd = open(m_path.c_str(), O_RDWR | O_CREAT, S_IRWXU);
    if (m_fd < 0) {
        LOGE("failed to open: %s , error:%d", m_path.c_str(), strerror(errno));
        return;
    }
    m_size = 0;
    struct stat st = {0};
    if (fstat(m_fd, &st) != -1) {
        m_size = static_cast<size_t>(st.st_size);
    }
    //resize file to default page size
    if (m_size < getpagesize() || (m_size % getpagesize() != 0)) {
        size_t pre = m_size;
        m_size = (m_size / getpagesize() + 1) * getpagesize();
        if (ftruncate(m_fd, m_size) != 0 || !fill_file_by_zero(m_fd, 0, m_size)) {
            LOGE("fail to truncate [%s] to size %zu, %s", m_path.c_str(), m_size,
                 strerror(errno));
            m_size = pre;
        }
        fill_file_by_zero(m_fd, pre, m_size - pre);
    }
    m_memory_ptr = (char *) mmap(nullptr, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
    if (m_memory_ptr == MAP_FAILED) {
        LOGE("fail to mmap [%s], %s", m_map_id.c_str(), strerror(errno));
        return;
    }
    //read file real size
    memcpy(&m_real_size, m_memory_ptr, 4);
    LOGE("loading [%s] with %zu size in total, file size is %zu",
         m_map_id.c_str(), m_real_size, m_size);

    IPCBuffer inputBuffer(m_memory_ptr + 4, m_real_size, false);
    m_dic.clear();
    ProtoBuffCoder::decode_buffer_to_map(m_dic, inputBuffer);

    m_sink_data = new SinkData(m_memory_ptr + 4 + m_real_size, m_size - 4 - m_real_size);
    LOGE("loaded [%s] with %zu values", m_map_id.c_str(), m_dic.size());
}


static string gen_map_key(const string &map_id, string *relative_path) {
    if (relative_path && s_root_dir != (*relative_path)) {
        return md5(*relative_path + "/" + map_id);
    }
    return map_id;
}

static string md5(const string &value) {
    unsigned char md[MD5_DIGEST_LENGTH] = {0};
    char tmp[3] = {0}, buf[33] = {0};
    MD5((const unsigned char *) value.c_str(), value.size(), md);
    for (unsigned char i : md) {
        sprintf(tmp, "%2.2x", i);
        strcat(buf, tmp);
    }
    return string(buf);
}


