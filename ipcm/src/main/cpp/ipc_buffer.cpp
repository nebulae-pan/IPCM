//
// Created by nebula on 2019-06-17.
//

#include "ipc_buffer.h"
#include <cstdlib>
#include <utility>

using namespace std;

IPCBuffer::IPCBuffer(size_t length) {
    if (length > 0) {
        m_ptr = malloc(length);
    }
}

IPCBuffer::IPCBuffer(void *source, size_t size, bool copy) : m_ptr(source), m_size(size),
                                                             m_copy_flag(copy) {
    if (m_copy_flag) {
        m_ptr = malloc(size);
        memcpy(m_ptr, source, size);
    }
}

IPCBuffer::IPCBuffer(IPCBuffer &&buffer) noexcept
        : m_ptr(buffer.m_ptr), m_size(buffer.m_size) {
    buffer.m_ptr = nullptr;
    buffer.m_size = 0;
}

IPCBuffer &IPCBuffer::operator=(IPCBuffer &&buffer) noexcept {
    m_ptr = buffer.m_ptr;
    m_size = buffer.m_size;
    buffer.m_ptr = nullptr;
    buffer.m_size = 0;
    return *this;
}

IPCBuffer::~IPCBuffer() {
    if (m_copy_flag && m_ptr) {
        free(m_ptr);
    }
    m_ptr = nullptr;
}


