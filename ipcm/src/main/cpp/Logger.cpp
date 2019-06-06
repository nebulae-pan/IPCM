//
// Created by nebula on 2019-06-06.
//

#include <string>
#include "Logger.h"

#define TAG "IPCM"
using namespace std;

void log(int priority, const char *file, int line, const char *func, const char *format, ...) {
    string msg;
    va_list list;
    char buffer[8];
    va_start(list, format);
    auto length = vsnprintf(buffer, sizeof(buffer), format, list);
    va_end(list);
    if (length < 0) {
        msg = {};
    } else if (length <= sizeof(buffer)) {
        msg = string(buffer, static_cast<unsigned long>(length));
    } else {
        //resize string
        msg.resize(static_cast<unsigned long>(length));
        va_start(list, format);
        vsnprintf(const_cast<char *>(msg.data()), static_cast<size_t> (length) + 1, format, list);
        va_end(list);
    }
    __android_log_print(priority, TAG, "[%s:%d %s] %s", file, line, func, msg.c_str());
}