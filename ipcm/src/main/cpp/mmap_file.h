//
// Created by nebula on 2019-06-08.
//

#ifndef IPCM_MEMORYMAPFILE_H
#define IPCM_MEMORYMAPFILE_H

#include "logger.h"
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;


class MemoryMapFile{
    string m_name;
    int m_fd;
    void * m_segment_ptr;
    size_t m_segment_size;

public:
    MemoryMapFile(const string &path,
                  size_t size = static_cast<size_t>(getpagesize()),
                  bool file_type = false);
    ~MemoryMapFile();
};

bool mk_path(char *path);

bool fill_file_by_zero(int fd, size_t start, size_t size);

void delete_file(const string &file_path);

#endif //IPCM_MEMORYMAPFILE_H
