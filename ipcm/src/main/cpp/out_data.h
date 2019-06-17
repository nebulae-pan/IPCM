//
// Created by nebula on 2019-06-17.
//

#ifndef IPCM_OUT_DATA_H
#define IPCM_OUT_DATA_H

#include "logger.h"
#include <cstdint>

class SinkData{
    uint8_t *m_ptr;
    size_t m_size;
    int32_t m_pos;
public:
    SinkData(void *ptr, size_t size);

    ~SinkData();

    void write_raw_byte(uint8_t data);

    void write_byte(bool data);

    void write_int32(int32_t data);
};

#endif //IPCM_OUT_DATA_H
