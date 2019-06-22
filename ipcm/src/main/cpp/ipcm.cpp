//
// Created by nebula on 2019-06-05.
//

#include "ipcm.h"
#include "mmap_file.h"
#include "thread_lock.h"
#include "constant.h"
#include <unordered_map>

using namespace std;
static unordered_map<string, IPCM *> *s_instance_dic;
static ThreadLock s_instance_lock;
static string s_root_dir;


static std::string md5(const std::string &value);
static std::string gen_map_key(const std::string &map_id, std::string *relative_path);

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
    LockUtil<ThreadLock> lock(&s_instance_lock);
    auto key = gen_map_key(map_id, nullptr);
    auto iterator = s_instance_dic->find(key);
    if (iterator != s_instance_dic->end()) {
        return iterator->second;
    }
    //todo:deal with relative path
    auto ptr = new IPCM();
    (*s_instance_dic)[key] = ptr;
    return ptr;
}

bool IPCM::encodeInt(const std::string &key, int value) {
    if (key.empty()) {
        return false;
    }
    size_t size = 1;
    IPCBuffer buffer(size);
    SinkData data(buffer.get_ptr(), size);
    data.write_int32(value);
    return set_memory_data_by_key(key, move(buffer));
}

bool IPCM::set_memory_data_by_key(const std::string key, IPCBuffer &&buffer) {
    if(buffer.length() == 0 || key.empty()){
        return false;
    }
    LockUtil lock(&m_thread_lock);
    LockUtil lock1(&m_exclusive_lock);
    auto iter = m_dic.find(key);
    if(iter == m_dic.end()){
        iter = m_dic.emplace(key, move(buffer)).first;
    } else{
        iter->second = move(buffer);
    }
    return true;
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


