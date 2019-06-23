//
// Created by nebula on 2019-06-11.
//

#ifndef IPCM_FILE_LOCK_H
#define IPCM_FILE_LOCK_H

#include "logger.h"
#include <fcntl.h>
#include <assert.h>

class FileLock {
    int m_fd;
    flock m_flock;
    size_t m_shared_lock_count;
    size_t m_exclusive_lock_count;
    //flock type  exclusive:F_WRLCK, shared:F_RDLCK
    short m_type;
    bool m_enable;

    bool do_lock(int cmd);

    bool is_lock_valid() {
        return m_fd >= 0;
    }

//    FileLock(const FileLock &other) = delete;

//    FileLock &operator=(const FileLock &other) = delete;

public:
    static const int TYPE_EXCLUSIVE = F_WRLCK;
    static const int TYPE_SHARED = F_RDLCK;
    FileLock(int fd, short type);

    bool lock();

    bool try_lock();

    bool unlock();
};

template<typename T>
class LockUtil {
    T *m_lock;

    LockUtil(const LockUtil<T> &other) = delete;

    LockUtil &operator=(const LockUtil<T> &other) = delete;
public:
    LockUtil(T *lock) : m_lock(lock) {
        assert(m_lock);
        this->lock();
    }

    ~LockUtil() {
        unlock();
        m_lock = nullptr;
    }

    void lock() {
        if (m_lock)m_lock->lock();
    }

    void unlock() {
        if (m_lock)m_lock->unlock();
    }
};

#define SCOPELOCK(lock) _SCOPELOCK(lock, __COUNTER__)
#define _SCOPELOCK(lock, counter) __SCOPELOCK(lock, counter)
#define __SCOPELOCK(lock, counter) LockUtil<decltype(lock)> __scopedLock##counter(&lock)


#endif //IPCM_FILE_LOCK_H
