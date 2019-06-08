//
// Created by nebula on 2019-06-05.
//

#include "ipcm.h"
#include "mmap_file.h"
#include "thread_lock.h"
#include <unordered_map>

using namespace std;
static unordered_map<string, IPCM*> *s_dic;
static ThreadLock s_lock;
static string s_root_dir;

void init_thread(){
    s_dic = new unordered_map<string, IPCM*>;
    s_lock = ThreadLock();
}

void IPCM::init(const std::string &root_dir) {
    static pthread_once_t run_once = PTHREAD_ONCE_INIT;
    pthread_once(&run_once, init_thread);

    s_root_dir = root_dir;
    char *path = strdup(s_root_dir.c_str());
    if (path) {
        mk_path(path);
        free(path);
    }

    LOGD("root dir: %s", s_root_dir.c_str());

}

