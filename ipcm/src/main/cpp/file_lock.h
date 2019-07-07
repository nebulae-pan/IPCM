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

    bool do_lock(short type, int cmd);

    bool is_lock_valid() {
        return m_fd >= 0;
    }

//    FileLock(const FileLock &other) = delete;

//    FileLock &operator=(const FileLock &other) = delete;

public:
    static const int TYPE_EXCLUSIVE = F_WRLCK;
    static const int TYPE_SHARED = F_RDLCK;

    FileLock(int fd);

    bool lock(short type);

    bool try_lock(short type);

    bool unlock(short type);
};

class ProcessLock {
    FileLock *m_lock;
    short m_type;
    bool m_enable;

public:
    ProcessLock(FileLock *file_lock, short type);

    void lock();

    void try_lock();

    void unlock();

    void set_enable(bool enable);
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

#define SCOPE_LOCK(lock) _SCOPE_LOCK(lock, __COUNTER__)
#define _SCOPE_LOCK(lock, counter) __SCOPE_LOCK(lock, counter)
#define __SCOPE_LOCK(lock, counter) LockUtil<decltype(lock)> __scopedLock##counter(&lock)


#endif //IPCM_FILE_LOCK_H
