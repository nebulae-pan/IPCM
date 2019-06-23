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
#include <unordered_map>
#include <string>
#include <pthread.h>

#define DEFAULT_MAP_KEY "ipcm.default"

class IPCM{
private:
    std::unordered_map<std::string, IPCBuffer> m_dic;
    char* m_memory_ptr;
    ThreadLock m_thread_lock;
    FileLock m_exclusive_lock;

public:
    static void init(const std::string &root_dir);

    static IPCM *default_instance(size_t mode);

    static IPCM *create_instance(const std::string &map_id, int page_size,
            size_t mode, std::string *relative_path = nullptr);

    IPCM();

    bool set_memory_data_by_key(const std::string &key, IPCBuffer &&buffer);

    IPCBuffer &get_memory_data_by_key(const std::string &key);

    bool append_data_by_key(const std::string &key, const IPCBuffer &buffer);

    bool encodeInt(const std::string &key, int value);

    int32_t decodeInt32(const std::string &key , int32_t default_value);
};
#endif //IPCM_IPCM_H

