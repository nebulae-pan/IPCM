//
// Created by nebula on 2019-06-22.
//

#include "source_data.h"
#include "sink_data.h"
#include "logger.h"

SourceData::SourceData(const void *ptr, size_t size) : m_ptr((uint8_t *) ptr), m_size(size),
                                                       m_pos(0) {

}

SourceData::~SourceData() {
    m_ptr = nullptr;
    m_pos = 0;
    m_size = 0;
}

uint8_t SourceData::read_byte() {
    if (m_pos == m_size) {
        LOGE("read byte out of range, position:%d, size:%d", m_pos, m_size);
    }
    return m_ptr[m_pos++];
}

int32_t SourceData::read_varint32() {
    int32_t result = read_byte();
    if ((static_cast<uint32_t >(result) & 0x80u) == 0) {
        return result;
    }
    for (uint8_t shift = 7; shift <= 28; shift += 7) {
        uint8_t temp = read_byte();
        if ((temp & 0x80u) != 0) {
            result |= ((temp & 0x7fu) << shift);
        } else {
            result |= (temp << shift);
        }
    }
    if (result < 0) {
        for (int i = 0; i < 5; i++) {
            if (read_byte() >= 0) {
                return result;
            }
        }
        LOGE("wrong protobuf varint32 negative number format.");
    }
    return result;
}

int64_t SourceData::read_varint64() {
    int64_t result = read_byte();
    if ((static_cast<uint64_t >(result) & 0x80u) == 0) {
        return result;
    }
    for (uint8_t shift = 7; shift < 64; shift += 7) {
        uint8_t temp = read_byte();
        if ((temp & 0x80u) != 0) {
            result |= ((uint64_t) (temp & 0x7fu) << shift);
        } else {
            result |= ((uint64_t) temp << shift);
        }
    }
    return result;
}

int32_t SourceData::read_int32() {
    return read_varint32();
}


int64_t SourceData::read_int64() {
    return read_varint64();
}

