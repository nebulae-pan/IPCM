//
// Created by nebula on 2019-06-11.
//

#include "file_lock.h"

FileLock::FileLock(int fd, short type) {

}

bool FileLock::do_lock() {
    return false;
}

bool FileLock::lock() {
    return false;
}

bool FileLock::try_lock() {
    return false;
}

bool FileLock::unlock() {
    return false;
}