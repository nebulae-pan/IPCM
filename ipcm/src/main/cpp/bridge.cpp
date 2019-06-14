//
// Created by nebula on 2019-06-05.
//
#include <jni.h>
#include "logger.h"
#include "ipcm.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_nebula_module_ipcm_IPCM_nativeInit(JNIEnv* env, jclass type, jstring rootDir){
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
extern "C"
JNIEXPORT jlong JNICALL
Java_com_nebula_module_ipcm_IPCM_defaultIPCM(JNIEnv *env, jclass type, jint mode) {
    IPCM *ptr = IPCM::default_instance(static_cast<size_t>(mode));
    return reinterpret_cast<jlong>(ptr);
}