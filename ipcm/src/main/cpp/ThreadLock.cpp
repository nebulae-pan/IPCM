//
// Created by nebula on 2019-06-06.
//

#include "ThreadLock.h"
#include "Logger.h"

ThreadLock::ThreadLock() {
    // use pthread init mutex reentrant lock
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_lock, &mutex_attr);
    //destroy attr
    pthread_mutexattr_destroy(&mutex_attr);
}

ThreadLock::~ThreadLock() {
    pthread_mutex_destroy(&m_lock);
}

void ThreadLock::lock() {
    auto result = pthread_mutex_lock(&m_lock);
    if(result != 0){
        LOGE("fail to lock %p. result=%d,errno=%d", &m_lock, result, strerror(errno));
    }
}

bool ThreadLock::try_lock() {
    auto result = pthread_mutex_trylock(&m_lock);
    if(result != 0){
        LOGE("fail to lock %p. result=%d,errno=%d", &m_lock, result, strerror(errno));
    }
    return result == 0;
}

void ThreadLock::unlock() {
    auto result = pthread_mutex_unlock(&m_lock);
    if(result != 0){
        LOGE("fail to lock %p. result=%d,errno=%d", &m_lock, result, strerror(errno));
    }
}
