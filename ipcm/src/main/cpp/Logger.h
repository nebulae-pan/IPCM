//
// Created by nebula on 2019-06-06.
//

#ifndef IPCM_LOGGER_H
#define IPCM_LOGGER_H

#include <android/log.h>
#include <cstring>
#include <errno.h>

//#define ERROR ANDROID_LOG_ERROR
#define __file__ (strrchr(__FILE__, '/') + 1)

#define LOGE(format, ...)  log(ANDROID_LOG_ERROR, __file__,__LINE__, __func__, format, ##__VA_ARGS__)
#define LOGW(format, ...)  log(ANDROID_LOG_WRAN, __file__,__LINE__, __func__, format, ##__VA_ARGS__)
#define LOGD(format, ...)  log(ANDROID_LOG_DEBUG, __file__,__LINE__, __func__, format, ##__VA_ARGS__)

void log(int priority, const char *file, int line, const char *func, const char *format, ...);

#endif //IPCM_LOGGER_H
