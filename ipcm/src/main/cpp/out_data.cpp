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
//        this->write_raw_byte()
    } else{
//        this->write_raw_byte()
    }
}
