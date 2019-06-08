//
// Created by nebula on 2019-06-05.
//
#include <jni.h>
#include "logger.h"
#include "ipcm.h"

extern "C" JNIEXPORT JNICALL void Java_com_nebula_module_ipcm_IPCM_nativeInit(JNIEnv* env, jobject object, jstring rootDir){
    if (!rootDir) {
        return;
    }
    const char *str = env->GetStringUTFChars(rootDir, nullptr);
    //use param init IPCM in c++ layer
    if (str) {
        IPCM::init(str);
        env->ReleaseStringUTFChars(rootDir, str);
    }
}
