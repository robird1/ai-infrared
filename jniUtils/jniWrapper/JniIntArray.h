#ifndef JNIINTARRAY_H
#define JNIINTARRAY_H

#include <jni.h>

class JniIntArray {
private:
    JNIEnv *env;
    jintArray jniPtr;
    jint *pArray;
    int len;

public:
    JniIntArray(JNIEnv *env, jintArray jniPtr);

    ~JniIntArray();

    int size();

    int *get();

    int &get(int k);

    int &operator[](int k);
};

#endif // JNIFLOATARRAY_H
