//
// Created by nebula on 2019-06-11.
//

#include "file_lock.h"

FileLock::FileLock(int fd, short type) : m_fd(fd), m_type(type) {
    m_flock.l_type = type;
    m_flock.l_len = 0;
    m_flock.l_whence = SEEK_SET;
    m_flock.l_start = 0;
    m_flock.l_pid = 0;
}

bool FileLock::do_lock(int cmd) {
    if (!is_lock_valid())return false;
    bool unlock_first_if_needed = false;
    //shared lock
    if (m_type == F_RDLCK) {
        m_shared_lock_count++;
        if (m_shared_lock_count > 1 || m_exclusive_lock_count > 0) {
            return true;
        }
    } else {
        m_exclusive_lock_count++;
        if(m_exclusive_lock_count > 1)return true;
        if(m_shared_lock_count > 0)unlock_first_if_needed = true;
    }

    m_flock.l_type = m_type;
    if (unlock_first_if_needed) {

    }
    auto result = fcntl(m_fd, cmd, &m_flock);
    if (result != 0) {
        LOGE("fail to lock fd=%d, ret=%d, error:%s", m_fd, result, strerror(errno));
        return false;
    }

    return true;
}

bool FileLock::lock() {
    return do_lock(F_SETLKW);
}

bool FileLock::try_lock() {
    return do_lock(F_SETLK);
}

bool FileLock::unlock() {
    return false;
}