//
// Created by nebula on 2019-06-05.
//
#include <jni.h>
#include "logger.h"
#include "ipcm.h"

static std::string jstring2string(JNIEnv *env, jstring str) {
    if (!str) {
        return "";
    }
    const char *chars = env->GetStringUTFChars(str, nullptr);
    if (!chars) {
        return "";
    }
    std::string result(chars);
    env->ReleaseStringUTFChars(str, chars);
    return result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nebula_module_ipcm_IPCM_nativeInit(JNIEnv *env, jclass type, jstring rootDir) {
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

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nebula_module_ipcm_IPCM_encodeInt(JNIEnv *env, jobject instance, jlong ptr, jstring key_,
                                           jint value) {
    IPCM *ipcm = reinterpret_cast<IPCM *>(ptr);
    if (ipcm && key_) {
        std::string key = jstring2string(env, key_);
        return static_cast<jboolean>(ipcm->encodeInt(key, value));
    }
    return static_cast<jboolean>(false);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nebula_module_ipcm_IPCM_encodeString(JNIEnv *env, jobject instance, jlong mPointer,
                                              jstring key_, jstring value_) {
    const char *key = env->GetStringUTFChars(key_, nullptr);
    const char *value = env->GetStringUTFChars(value_, nullptr);

    // TODO

    env->ReleaseStringUTFChars(key_, key);
    env->ReleaseStringUTFChars(value_, value);
}