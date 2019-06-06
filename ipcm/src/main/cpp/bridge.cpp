//
// Created by nebula on 2019-06-05.
//
#include <jni.h>
#include "Logger.h"

extern "C" JNIEXPORT JNICALL void Java_com_nebula_module_ipcm_IPCM_init(JNIEnv* env, jobject object){
    LOGE("%d string...", 123);
}
