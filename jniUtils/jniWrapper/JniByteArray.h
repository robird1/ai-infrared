#ifndef JNIBYTEARRAY_H
#define JNIBYTEARRAY_H

#include <jni.h>

class JniByteArray {
private:
    JNIEnv *env;
    jbyteArray jniPtr;
    jbyte *pArray;
    int len;

public:
    JniByteArray(JNIEnv *env, jbyteArray jniPtr);

    ~JniByteArray();

    int size();

    jbyte *get();

    jbyte &get(int k);

    jbyte &operator[](int k);
};

#endif // JNIFLOATARRAY_H
