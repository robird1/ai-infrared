#include "JniByteArray.h"

JniByteArray::JniByteArray(JNIEnv *env, jbyteArray jniPtr) {
    this->env = env;
    this->jniPtr = jniPtr;
    this->len = 0;
    this->pArray = NULL;
    if (NULL != jniPtr) {
        len = env->GetArrayLength(jniPtr);
        pArray = env->GetByteArrayElements(jniPtr, NULL);
    }
}

JniByteArray::~JniByteArray() {
    if (pArray != NULL && jniPtr != NULL) {
        env->ReleaseByteArrayElements(jniPtr, pArray, 0);
    }
}

int JniByteArray::size() {
    return len;
}

jbyte *JniByteArray::get() {
    return this->pArray;
}

jbyte &JniByteArray::get(int k) {
    return this->pArray[k];
}

jbyte &JniByteArray::operator[](int k) {
    return get(k);
}