#include "JniByteArray.h"
#include "JniIntArray.h"

JniIntArray::JniIntArray(JNIEnv *env, jintArray jniPtr) {
    this->env = env;
    this->jniPtr = jniPtr;
    this->len = 0;
    this->pArray = NULL;
    if (NULL != jniPtr) {
        len = env->GetArrayLength(jniPtr);
        pArray = env->GetIntArrayElements(jniPtr, NULL);
    }
}

JniIntArray::~JniIntArray() {
    if (pArray != NULL && jniPtr != NULL) {
        env->ReleaseIntArrayElements(jniPtr, pArray, 0);
    }
}

int JniIntArray::size() {
    return len;
}

int *JniIntArray::get() {
    return this->pArray;
}

int &JniIntArray::get(int k) {
    return this->pArray[k];
}

int &JniIntArray::operator[](int k) {
    return get(k);
}