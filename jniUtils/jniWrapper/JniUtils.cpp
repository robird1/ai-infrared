#include <unistd.h>
#include <vector>
#include "JniUtils.h"
#include "JniString.h"
#include "macUtil.h"
#include "JniByteArray.h"

#define LICENSE_FILENAME "license.txt"

jobject JniUtils::getCurrentApplication(JNIEnv *env)
{
    jclass jclsActivityThread = env->FindClass("android/app/ActivityThread");
    jmethodID jmidCurrentApplication = env->GetStaticMethodID(jclsActivityThread, "currentApplication", "()Landroid/app/Application;");
    jobject context = env->CallStaticObjectMethod(jclsActivityThread, jmidCurrentApplication);
    return context;
}

std::string JniUtils::getCacheDir(JNIEnv *env) {
    std::string strCacheDir;
    do {
        if(env == NULL) {
            char buf_pwd[512] = {0};
            getcwd(buf_pwd, 512);
            strCacheDir = buf_pwd;
//            strCacheDir += getFileSeparator(env);
//            strCacheDir += "cache";
            break;
        }

        jobject context = getCurrentApplication(env);
        if (context == NULL) {
            break;
        }

        jclass jclsContext = env->FindClass("android/content/Context");
        jmethodID jmidGetCacheDir = env->GetMethodID(jclsContext, "getCacheDir", "()Ljava/io/File;");
        jobject jfileCacheDir = env->CallObjectMethod(context, jmidGetCacheDir);
        if (jfileCacheDir == NULL) {
            break;
        }

        jclass jclsFile = env->FindClass("java/io/File");
        jmethodID jmidToString = env->GetMethodID(jclsFile, "toString", "()Ljava/lang/String;");
        jstring jsCacheDir = static_cast<jstring>(env->CallObjectMethod(jfileCacheDir, jmidToString));

        strCacheDir = jstring2string(env, jsCacheDir);
    } while(0);
    return strCacheDir;
}

string JniUtils::getFileSeparator(JNIEnv *env) {
    std::string strFileSeparator("/");
    do {
        if(env == NULL) {
            break;
        }
        jclass jclsFile = env->FindClass("java/io/File");
        if (jclsFile == NULL) {
            break;
        }
        jfieldID jfSeparator = env->GetStaticFieldID(jclsFile, "separator", "Ljava/lang/String;");
        jstring jsSeparator = (jstring)(env->GetStaticObjectField(jclsFile, jfSeparator));
        strFileSeparator = jstring2string(env, jsSeparator);
    } while(0);

    return strFileSeparator;
}

std::string JniUtils::jstring2string(JNIEnv *env, jstring jstr) {
    JniString jniStr(env, jstr);
    return std::string(jniStr.get());
}

std::vector<char> JniUtils::jbyteArray2VectorChar(JNIEnv *env, jbyteArray jbyteArray) {
    JniByteArray byteArray(env, jbyteArray);
    jbyte *ptr = byteArray.get();
    return std::vector<char>(ptr, ptr + byteArray.size());
}

static std::string jbyteArrayToString(JNIEnv *env, jbyteArray jbyteArray){
    JniByteArray byteArray(env, jbyteArray);
    jbyte *pKey = byteArray.get();
    return std::string(pKey, pKey + byteArray.size());
}

std::string JniUtils::getAndroidId(JNIEnv *env)
{
    std::string strAndroidId;
    do {
        if(env == NULL) {
            strAndroidId = uls_getLocalMac();
            break;
        }

        jobject context = getCurrentApplication(env);
        if (context == NULL) {
            break;
        }

        jclass jclsContext = env->FindClass("android/content/Context");
        jmethodID jmidGetContentResolver = env->GetMethodID(jclsContext, "getContentResolver", "()Landroid/content/ContentResolver;");
        jobject contentResolver = env->CallObjectMethod(context, jmidGetContentResolver);
        if (contentResolver == NULL) {
            break;
        }

        jclass jclsSettingSecure = env->FindClass("android/provider/Settings$Secure");
        if (jclsSettingSecure == NULL) {
            break;
        }

        jmethodID jmidGetString = env->GetStaticMethodID(jclsSettingSecure, "getString", "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");
        if (jmidGetString == NULL) {
            break;
        }

        jfieldID jfANDROID_ID = env->GetStaticFieldID(jclsSettingSecure, "ANDROID_ID", "Ljava/lang/String;");
        jstring jsANDROID_ID = (jstring)(env->GetStaticObjectField(jclsSettingSecure, jfANDROID_ID));

        jstring jsAndroidId = static_cast<jstring>(env->CallStaticObjectMethod(jclsSettingSecure, jmidGetString, contentResolver, jsANDROID_ID));

        strAndroidId = jstring2string(env, jsAndroidId);
    } while(0);

    return strAndroidId;
}

std::string JniUtils::getPackageName(JNIEnv *env) {
    std::string strPackageName;
    do {
        if(env == NULL) {
            strPackageName = uls_getOwnerExeName();
            break;
        }

        jobject context = getCurrentApplication(env);
        if (context == NULL) {
            break;
        }

        jclass jclsContext = env->FindClass("android/content/Context");
        jmethodID jmidGetPackageName = env->GetMethodID(jclsContext, "getPackageName", "()Ljava/lang/String;");
        jstring jsPackageName = static_cast<jstring>(env->CallObjectMethod(context, jmidGetPackageName));

        strPackageName = jstring2string(env, jsPackageName);
    } while(0);
    return strPackageName;
}

std::string JniUtils::getLicensePath(JNIEnv *env) {
    std::string  strCacheDir = getCacheDir(env);
    return strCacheDir + getFileSeparator(env) + LICENSE_FILENAME;
}

jobject JniUtils::generateBitmap(JNIEnv *env, uint32_t width, uint32_t height) {
    jclass bitmapCls = env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapFunction = env->GetStaticMethodID(bitmapCls,
                                                            "createBitmap",
                                                            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jstring configName = env->NewStringUTF("ARGB_8888");
    jclass bitmapConfigClass = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID valueOfBitmapConfigFunction = env->GetStaticMethodID(
            bitmapConfigClass, "valueOf",
            "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");

    jobject bitmapConfig = env->CallStaticObjectMethod(bitmapConfigClass,
                                                       valueOfBitmapConfigFunction, configName);

    jobject newBitmap = env->CallStaticObjectMethod(bitmapCls,
                                                    createBitmapFunction,
                                                    width,
                                                    height, bitmapConfig);

    return newBitmap;
}