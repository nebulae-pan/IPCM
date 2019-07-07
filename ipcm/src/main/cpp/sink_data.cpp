#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
//
// Created by nebula on 2019-06-17.
//

#include "sink_data.h"
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

void SinkData::write_bool(bool data) {
    this->write_raw_byte(static_cast<uint8_t>(data ? 1 : 0));
}


void SinkData::write_int32(int32_t data) {
    if (data >= 0) {
        this->write_varint32(data);
    } else {
        this->write_varint64(static_cast<int64_t >(data));
    }
}

void SinkData::write_int64(int64_t data) {
    write_varint64(data);
}

void SinkData::write_varint32(int32_t value) {
    auto v = static_cast<uint32_t >(value);
    while (v > 0x7F) {
        this->write_raw_byte(static_cast<uint8_t>((v & 0x7F) | 0x80));
        v >>= 7;
    }
    this->write_raw_byte(static_cast<uint8_t >(v & 0x7F));
}

void SinkData::write_varint64(int64_t value) {
    auto v = static_cast<uint64_t>(value);
    while (v > 0x7F) {
        this->write_raw_byte(static_cast<uint8_t >((v & 0x7F) | 0x80));
        v >>= 7;
    }
    this->write_raw_byte(static_cast<uint8_t>(v & 0x7F));
}

void SinkData::write_string(const std::string &string) {
    size_t size = string.size();
    write_varint32(static_cast<int32_t>(size));
    memcpy(m_ptr + m_pos, string.data(), size);
    m_pos += size;
}

void SinkData::write_buff(const IPCBuffer &buffer) {
    size_t size = buffer.length();
    write_varint32(static_cast<int32_t>(size));
    write_raw_buff(buffer, size);
}

void SinkData::write_raw_buff(const IPCBuffer &buffer, size_t i) {
    memcpy(m_ptr, buffer.get_ptr(), i);
    m_pos += i;
}


#pragma clang diagnostic pop