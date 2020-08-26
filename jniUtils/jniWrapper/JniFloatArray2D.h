#ifndef JNIFLOATARRAY2D_H
#define JNIFLOATARRAY2D_H

#include "JniFloatArray.h"
#include <vector>

class JniFloatArray2D {
private:
    JNIEnv *env;
    jobjectArray jniPtr;
    std::vector<JniFloatArray *> vecFloatArray;
    int len;

public:
    JniFloatArray2D(JNIEnv *env, jobjectArray jniPtr);

    ~JniFloatArray2D();

    int size();

    JniFloatArray *&get(int k);

    JniFloatArray *&operator[](int k);

    std::vector<std::vector<float>> toVector();
};

#endif // JNIFLOATARRAY2D_H
