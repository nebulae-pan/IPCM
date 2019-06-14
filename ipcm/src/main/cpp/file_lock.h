//
// Created by nebula on 2019-06-11.
//

#ifndef IPCM_FILE_LOCK_H
#define IPCM_FILE_LOCK_H

#include "logger.h"
#include <fcntl.h>

class FileLock{
    int m_fd;
    flock m_flock;
    size_t m_shared_lock_count;
    size_t m_exclusive_lock_count;
    //flock type  exclusive:F_WRLCK, shared:F_RDLCK
    short m_type;
    bool m_enable;

    bool do_lock(int cmd);

    bool is_lock_valid(){
        return m_fd >= 0;
    }

public:
    FileLock(int fd, short type);
    bool lock();
    bool try_lock();
    bool unlock();
};

template <typename T>
class LockUtil{
    T *m_lock;
public:
    LockUtil(T *lock):m_lock(lock){
        this->lock();
    }

    ~LockUtil(){
        unlock();
        m_lock = nullptr;
    }

    void lock(){
        if(m_lock)m_lock->lock();
    }

    void unlock(){
        if(m_lock)m_lock->unlock();
    }
};


#endif //IPCM_FILE_LOCK_H
