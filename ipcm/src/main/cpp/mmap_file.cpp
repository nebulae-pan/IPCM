//
// Created by nebula on 2019-06-08.
//

#include "mmap_file.h"
#include "file_lock.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

MemoryMapFile::MemoryMapFile(const string &path, size_t size, bool file_type)
        : m_name(path), m_fd(-1), m_segment_ptr(nullptr), m_segment_size(0) {
    m_fd = open(path.c_str(), O_RDWR | O_CREAT , S_IRWXU);
    if(m_fd < 0){
        LOGE("failed to open %s, %s", m_name.c_str(), strerror(errno));
        return;
    }
    FileLock fileLock(m_fd, F_WRLCK);

    struct stat st = {};
    if (fstat(m_fd, &st) != -1) {
        m_segment_size = static_cast<size_t>(st.st_size);
    }
    if (m_segment_size < DEFAULT_MMAP_SIZE) {
        m_segment_size = static_cast<size_t>(DEFAULT_MMAP_SIZE);
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

}

void mk_path(const char *path) {

}