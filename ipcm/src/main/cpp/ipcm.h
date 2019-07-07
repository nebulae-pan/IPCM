//
// Created by nebula on 2019-06-05.
//

#ifndef IPCM_IPCM_H
#define IPCM_IPCM_H

#include "logger.h"
#include "file_lock.h"
#include "openssl/md5.h"
#include "ipc_buffer.h"
#include "sink_data.h"
#include "source_data.h"
#include "thread_lock.h"
#include "mmap_file.h"
#include "meta_info.h"
#include "proto_buff_coder.h"
#include <sys/stat.h>
#include <unordered_map>
#include <string>
#include <pthread.h>

#define DEFAULT_MAP_KEY "ipcm.default"

class IPCM {
private:
    std::unordered_map<std::string, IPCBuffer> m_dic;
    std::string m_path;
    std::string m_map_id;
    int m_fd;
    size_t m_size;
    size_t m_real_size;
    char *m_memory_ptr;
    ThreadLock m_thread_lock;

    FileLock m_lock;
    ProcessLock m_shared_lock;
    ProcessLock m_exclusive_lock;

    SinkData *m_sink_data;

    MetaInfo m_meta_info;
    MemoryMapFile m_memory_map_file;

    void write_real_size(size_t real_size);

    void load_file_data();

    bool set_memory_data_by_key(const std::string &key, IPCBuffer &&buffer);

    IPCBuffer &get_memory_data_by_key(const std::string &key);

public:
    static void init(const std::string &root_dir);

    static IPCM *default_instance(size_t mode);

    static IPCM *create_instance(const std::string &map_id, int page_size,
                                 size_t mode, std::string *relative_path = nullptr);

    IPCM(const std::string &m_map_id, int size);

    bool append_data_by_key(const std::string &key, const IPCBuffer &buffer);

    bool encode_int32(const std::string &key, int32_t value);

    bool encode_int64(const std::string &key, int64_t value);

    int32_t decode_int32(const std::string &key, int32_t default_value);

    int64_t decode_int64(const std::string &key, int64_t default_value);
};

#endif //IPCM_IPCM_H

