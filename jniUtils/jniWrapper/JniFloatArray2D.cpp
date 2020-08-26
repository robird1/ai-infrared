#include "JniFloatArray2D.h"

JniFloatArray2D::JniFloatArray2D(JNIEnv *env, jobjectArray jniPtr) {
    this->env = env;
    this->jniPtr = jniPtr;
    this->len = 0;
    this->vecFloatArray.clear();
    if (NULL != jniPtr) {
        len = env->GetArrayLength(jniPtr);
        for (int i = 0; i < len; i++) {
            jfloatArray arrayFloat = (jfloatArray) env->GetObjectArrayElement(jniPtr, i);
            JniFloatArray *pJniFloatArray = new JniFloatArray(env, arrayFloat);
            this->vecFloatArray.push_back(pJniFloatArray);
        }
    }
}

JniFloatArray2D::~JniFloatArray2D() {
    for (int i = 0; i < len; i++) {
        JniFloatArray *pJniFloatArray = this->vecFloatArray[i];
        if (pJniFloatArray != NULL) {
            delete pJniFloatArray;
        }
    }
    this->vecFloatArray.clear();
}

int JniFloatArray2D::size() {
    return len;
}

JniFloatArray *&JniFloatArray2D::get(int k) {
    return this->vecFloatArray[k];
}

JniFloatArray *&JniFloatArray2D::operator[](int k) {
    return get(k);
}

std::vector<std::vector<float>> JniFloatArray2D::toVector() {
    std::vector<std::vector<float>> vecArray2D;
    for(int i = 0; i < len; i++) {
        vecArray2D.push_back(vecFloatArray[i]->toVector());
    }
    return vecArray2D;
}
