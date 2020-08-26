#ifndef JNIFLOATARRAY_H
#define JNIFLOATARRAY_H

#include <jni.h>
#include <vector>

class JniFloatArray {
private:
    JNIEnv *env;
    jfloatArray jniPtr;
    float *pArray;
    int len;

public:
    JniFloatArray(JNIEnv *env, jfloatArray jniPtr);

    ~JniFloatArray();

    int size();

    float *get();

    float &get(int k);

    float &operator[](int k);

    std::vector<float> toVector();
};

#endif // JNIFLOATARRAY_H
