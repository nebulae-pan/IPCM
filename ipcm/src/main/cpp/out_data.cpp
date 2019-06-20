//
// Created by nebula on 2019-06-17.
//

#include "out_data.h"
#include <cassert>

SinkData::SinkData(void *ptr, size_t size) : m_ptr(static_cast<uint8_t *>(ptr)), m_size(size),
                                             m_pos(0) {
    assert(m_ptr);
}

SinkData::~SinkData() {
    m_ptr = nullptr;
    m_pos = 0;
}

void SinkData::write_raw_byte(uint8_t data) {
    if (m_pos == m_size) {
        LOGE("sink data write error. size:%d", m_pos);
        return;
    }
    m_ptr[m_pos++] = data;
}

void SinkData::write_byte(bool data) {
    this->write_raw_byte(static_cast<uint8_t>(data ? 1 : 0));
}


void SinkData::write_int32(int32_t data) {
    if (data >= 0) {
        this->write_varint_32(data);
    } else {
//        this->write_raw_byte()
    }
}

void SinkData::write_varint_32(int32_t value) {
    while (value > 0x7F) {
        this->write_raw_byte((static_cast<uint8_t>(value) & 0x7F) | 0x80);
        value >>= 7;
    }
    this->write_raw_byte(static_cast<uint8_t>(value) & 0x7F);
}

void SinkData::write_varint_64(int64_t value) {
    while (value > 0x7F) {
        this->write_raw_byte((static_cast<uint8_t>(value) & 0x7F) | 0x80);
        value >>= 7;
    }
    this->write_raw_byte(static_cast<uint8_t>(value) & 0x7F);
}

