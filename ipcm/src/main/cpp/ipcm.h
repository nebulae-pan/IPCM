//
// Created by nebula on 2019-06-05.
//

#ifndef IPCM_IPCM_H
#define IPCM_IPCM_H

#include "logger.h"
#include "file_lock.h"
#include "openssl/md5.h"
#include <unordered_map>
#include <string>
#include <pthread.h>

#define DEFAULT_MAP_KEY "ipcm.default"

class IPCM{

public:
    static void init(const std::string &root_dir);

    static IPCM *default_instance(size_t mode);

    static IPCM *create_instance(const std::string &map_id, int page_size,
            size_t mode, std::string *relative_path = nullptr);
};

static std::string md5(const std::string &value);

static std::string gen_map_key(const std::string &map_id, std::string *relative_path);

#endif //IPCM_IPCM_H

