//
// Created by nebula on 2019-06-11.
//

#ifndef IPCM_FILE_LOCK_H
#define IPCM_FILE_LOCK_H

#include <fcntl.h>

class FileLock{
    int m_fd;
    flock m_flock;
    size_t m_shared_lock_count;
    size_t m_exclusive_lock_count;
    //flock type  exclusive:F_WRLCK, shared:F_RDLCK
    short type;

    bool do_lock();

    bool is_lock_valid(){
        return m_fd >= 0;
    }

public:
    FileLock(int fd, short type);
    bool lock();
    bool try_lock();
    bool unlock();


};


#endif //IPCM_FILE_LOCK_H
