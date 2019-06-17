//
// Created by nebula on 2019-06-17.
//

#include "ipc_buffer.h"
#include <cstdlib>

IPCBuffer::IPCBuffer(size_t length) {
    if (length > 0) {
        m_ptr = malloc(length);
    }
}

IPCBuffer::IPCBuffer(void *source, size_t size) {

}


