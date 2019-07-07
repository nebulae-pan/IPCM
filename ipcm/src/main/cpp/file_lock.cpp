//
// Created by nebula on 2019-06-11.
//

#include "file_lock.h"

FileLock::FileLock(int fd) : m_fd(fd) {
    m_flock.l_type = F_WRLCK;
    m_flock.l_len = 0;
    m_flock.l_whence = SEEK_SET;
    m_flock.l_start = 0;
    m_flock.l_pid = 0;
}

bool FileLock::do_lock(short type, int cmd) {
    if (!is_lock_valid())return false;
    bool unlock_first_if_needed = false;
    //shared lock
    if (type == F_RDLCK) {
        m_shared_lock_count++;
        if (m_shared_lock_count > 1 || m_exclusive_lock_count > 0) {
            return true;
        }
    } else {
        m_exclusive_lock_count++;
        //already lock writeLock
        if (m_exclusive_lock_count > 1)return true;
        //need upgrade readLock to writeLock
        if (m_shared_lock_count > 0)unlock_first_if_needed = true;
    }

    m_flock.l_type = type;
    if (unlock_first_if_needed) {
        //need upgrade readLock to writeLock，so unlock exist readLock and lock writeLock
        //first try lock, if false, unlock readLock
        auto result = fcntl(m_fd, F_SETLK, &m_flock);
        if (result == 0) {
            return true;
        }
        auto type = m_flock.l_type;
        m_flock.l_type = F_UNLCK;
        result = fcntl(m_fd, F_SETLK, m_flock);
        if (result != 0) {
            LOGE("fail to try unlock first fd=%d, ret=%d, error:%s", m_fd, result, strerror(errno));
        }
        m_flock.l_type = type;
    }
    auto result = fcntl(m_fd, cmd, &m_flock);
    if (result != 0) {
        LOGE("fail to lock fd=%d, ret=%d, error:%s", m_fd, result, strerror(errno));
        return false;
    }
    return true;
}

bool FileLock::lock(short type) {
    return do_lock(type, F_SETLKW);
}

bool FileLock::try_lock(short type) {
    return do_lock(type, F_SETLK);
}

bool FileLock::unlock(short type) {
    if (!is_lock_valid()) {
        return false;
    }
    bool downgrade_to_read_lock = false;
    if (type == F_RDLCK) {
        if (m_shared_lock_count == 0) {
            return false;
        }
        m_shared_lock_count--;
        if (m_shared_lock_count > 0 || m_exclusive_lock_count > 0) {
            return true;
        }
    } else {
        if (m_exclusive_lock_count == 0) {
            return false;
        }
        m_exclusive_lock_count--;
        if (m_exclusive_lock_count > 0) {
            return true;
        }
        if (m_shared_lock_count > 0)downgrade_to_read_lock = true;
    }

    m_flock.l_type = static_cast<short>(downgrade_to_read_lock ? F_RDLCK : F_UNLCK);
    //writeLock downgrade
    auto result = fcntl(m_fd, F_SETLK, m_flock);
    if (result == 0) {
        return true;
    }
    LOGE("fail to unlock. fd=%d, result=%d, error=%s", m_fd, result, strerror(errno));
    return false;
}

ProcessLock::ProcessLock(FileLock *file_lock, short type)
        : m_lock(file_lock), m_type(type), m_enable(true) {

}

void ProcessLock::lock() {
    if (m_enable) {
        m_lock->lock(m_type);
    }
}

void ProcessLock::try_lock() {
    if (m_enable) {
        m_lock->try_lock(m_type);
    }
}

void ProcessLock::unlock() {
    if (m_enable) {
        m_lock->unlock(m_type);
    }
}

void ProcessLock::set_enable(bool enable) {
    m_enable = enable;
}
