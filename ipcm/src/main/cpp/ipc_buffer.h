//
// Created by nebula on 2019-06-17.
//

#ifndef IPCM_IPC_BUFFER_H
#define IPCM_IPC_BUFFER_H

#include <cstdint>

class IPCBuffer{
private:
    void *m_ptr;
    size_t m_size;
public:
    void *get_ptr() const { return m_ptr;}
    size_t length() const { return m_size; }

    IPCBuffer(size_t length = 0);

    IPCBuffer(void *source, size_t size);
};

#endif //IPCM_IPC_BUFFER_H
