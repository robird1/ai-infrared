#include "JniString.h"
#include <jni.h>

JniString::JniString(JNIEnv *env, jstring jniPtr) {
    this->env = env;
    this->jniStr = jniPtr;
    this->pStr = NULL;
    if (NULL != jniPtr) {
        this->pStr = env->GetStringUTFChars(jniPtr, 0);
    }
}

JniString::~JniString() {
    if (pStr != NULL && jniStr != NULL) {
        env->ReleaseStringUTFChars(jniStr, pStr);
    }
}

const char *JniString::get() {
    return this->pStr;
}
