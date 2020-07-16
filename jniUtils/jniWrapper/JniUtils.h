#ifndef JNIUTILS_H
#define JNIUTILS_H

#include <jni.h>
#include <string>

class JniUtils {
public:
    static std::string getAndroidId(JNIEnv *env);
    static std::string getPackageName(JNIEnv *env);
    static std::string getLicensePath(JNIEnv *env);
    static jobject generateBitmap(JNIEnv *env, uint32_t width, uint32_t height);
    static std::string jstring2string(JNIEnv *env, jstring jstr);
    static std::vector<char> jbyteArray2VectorChar(JNIEnv *env, jbyteArray jbyteArray);
    static std::string jbyteArrayToString(JNIEnv *env, jbyteArray jbyteArray);

private:
    static std::string getCacheDir(JNIEnv *env);
    static std::string getFileSeparator(JNIEnv *env);
    static jobject getCurrentApplication(JNIEnv *env);

};

#endif // JNIUTILS_H
