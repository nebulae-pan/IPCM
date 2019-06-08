//
// Created by nebula on 2019-06-05.
//

#ifndef IPCM_IPCM_H
#define IPCM_IPCM_H

#include "logger.h"

#include <unordered_map>
#include <string>
#include <pthread.h>

class IPCM{

public:
    static void init(const std::string &root_dir);
};

#endif //IPCM_IPCM_H

