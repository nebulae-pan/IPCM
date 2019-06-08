//
// Created by nebula on 2019-06-08.
//

#ifndef IPCM_MEMORYMAPFILE_H
#define IPCM_MEMORYMAPFILE_H

#include <string>
#include <sys/ioctl.h>

using namespace std;

extern const int DEFAULT_MMAP_SIZE;

class MemoryMapFile{
    string m_name;
    int m_fd;
    void * m_segment_ptr;
    size_t m_segment_size;

public:
    MemoryMapFile(const string &path,
                  size_t size = static_cast<size_t>(DEFAULT_MMAP_SIZE),
                  bool file_type = false);
    ~MemoryMapFile();
};

void mk_path(const char *path);


#endif //IPCM_MEMORYMAPFILE_H
