//
// Created by nebula on 2019-06-06.
//

#ifndef IPCM_THREADLOCK_H
#define IPCM_THREADLOCK_H

#include <pthread.h>

class ThreadLock{
private:
    pthread_mutex_t m_lock;
public:
    ThreadLock();
    ~ThreadLock();

    void lock();
    bool try_lock();
    void unlock();
};

#endif //IPCM_THREADLOCK_H
