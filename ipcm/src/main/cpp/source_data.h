//
// Created by nebula on 2019-06-22.
//

#ifndef IPCM_BUFFER_H
#define IPCM_BUFFER_H

#include <cstdint>
#include <cstdlib>

class SourceData {
    uint8_t *m_ptr;
    size_t m_size;
    int32_t m_pos;

    uint8_t read_byte();

    int32_t read_varint32();

    int64_t read_varint64();

public:
    SourceData(const void *ptr, size_t size);

    ~SourceData();

    int32_t read_int32();

    int64_t read_int64();
};

#endif //IPCM_BUFFER_H
