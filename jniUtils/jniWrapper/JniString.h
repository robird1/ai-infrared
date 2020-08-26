#ifndef JNISTRING_H
#define JNISTRING_H

#include <jni.h>
#include <string>

using namespace std;

class JniString {
private:
    JNIEnv *env;
    jstring jniStr;
    const char *pStr;

public:
    JniString(JNIEnv *env, jstring jniPtr);

    ~JniString();

    const char *get();
};

#endif // JNISTRING_H
