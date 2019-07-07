//
// Created by nebula on 2019-06-08.
//

#include "mmap_file.h"
#include "file_lock.h"
#include <fcntl.h>
#include <sys/stat.h>

MemoryMapFile::MemoryMapFile(const string &path, size_t size, bool file_type)
        : m_name(path), m_fd(-1), m_segment_ptr(nullptr), m_segment_size(0) {
    m_fd = open(path.c_str(), O_RDWR | O_CREAT, S_IRWXU);
    if (m_fd < 0) {
        LOGE("failed to open %s, %s", m_name.c_str(), strerror(errno));
        return;
    }
    FileLock file_lock(m_fd);
    //lock file by fileLock
    ProcessLock lock(&file_lock, FileLock::TYPE_EXCLUSIVE);
    SCOPE_LOCK(lock);

    struct stat st = {};
    if (fstat(m_fd, &st) != -1) {
        //get segment size
        m_segment_size = static_cast<size_t>(st.st_size);
    }
    //resize file to default page size
    if (m_segment_size < getpagesize()) {
        m_segment_size = static_cast<size_t>(getpagesize());
        if (ftruncate(m_fd, m_segment_size) != 0 || !fill_file_by_zero(m_fd, 0, m_segment_size)) {
            LOGE("fail to truncate [%s] to size %zu, %s", m_name.c_str(), m_segment_size,
                 strerror(errno));
            close(m_fd);
            m_fd = -1;
            delete_file(m_name);
            return;
        }
    }
    m_segment_ptr =
            (char *) mmap(nullptr, m_segment_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
    if (m_segment_ptr == MAP_FAILED) {
        LOGE("fail to mmap [%s], %s", m_name.c_str(), strerror(errno));
        close(m_fd);
        m_fd = -1;
        m_segment_ptr = nullptr;
    }
}

MemoryMapFile::~MemoryMapFile() {
    if (m_segment_ptr != MAP_FAILED && m_segment_ptr != nullptr) {
        munmap(m_segment_ptr, m_segment_size);
        m_segment_ptr = nullptr;
    }
    if (m_fd >= 0) {
        close(m_fd);
        m_fd = -1;
    }
}

bool MemoryMapFile::is_file_valid() {
    return m_fd >= 0 && m_segment_size > 0 && m_segment_ptr && m_segment_ptr != MAP_FAILED;
}

void *MemoryMapFile::get_ptr() {
    return m_segment_ptr;
}

size_t MemoryMapFile::size() {
    return m_segment_size;
}

/**
 *
 * @param path
 * @return
 */
bool mk_path(char *path) {
    struct stat st = {};
    bool done = false;
    char *slash = path;
    while (!done) {
        //skip first '/'
        slash += strspn(slash, "/");
        //skip to next '/'
        slash += strcspn(slash, "/");

        done = (*slash == '\0');
        //split prev path
        *slash = '\0';
        if (stat(path, &st) != 0) {
            if (errno != ENOENT || mkdir(path, 0777) != 0) {
                LOGE("%s : %s", path, strerror(errno));
                return false;
            }
        } else if (!S_ISDIR(st.st_mode)) {
            LOGE("%s: %s", path, strerror(ENOTDIR));
            return false;
        }
        *slash = '/';
    }
    return true;
}

bool fill_file_by_zero(int fd, size_t start_pos, size_t size) {
    if (fd < 0) {
        return false;
    }
    if (lseek(fd, start_pos, SEEK_SET) < 0) {
        LOGE("fail to lseek fd:[%d] , error: %s", fd, strerror(errno));
        return false;
    }
    static const char zeros[4086] = {0};
    while (size >= sizeof(zeros)) {
        if (write(fd, zeros, sizeof(zeros)) < 0) {
            LOGE("fail to write fd:[%d], error:%s", fd, strerror(errno));
            return false;
        }
        size -= sizeof(zeros);
    }
    if (size > 0) {
        if (write(fd, zeros, size) < 0) {
            LOGE("fail to write fd:[%d], error:%s", fd, strerror(errno));
            return false;
        }
    }
    return true;
}

void delete_file(const string &file_path) {
    auto result = unlink(file_path.c_str());
    if (result != 0) {
        LOGE("failed to delete file:%s, %s", file_path.c_str(), strerror(errno));
    }
}