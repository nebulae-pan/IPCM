//
// Created by nebula on 2019-06-23.
//

#ifndef IPCM_META_INFO_H
#define IPCM_META_INFO_H

#include <cstdint>

struct MetaInfo {
    uint32_t m_version = 1;
    uint32_t m_write_count = 0;

    void write(void *ptr) {
        memcpy(ptr, this, sizeof(MetaInfo));
    }

    void read(void *ptr) {
        memcpy(this, ptr, sizeof(MetaInfo));
    }
};

#endif //IPCM_META_INFO_H
