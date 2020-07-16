#ifndef JNIBITMAP_H
#define JNIBITMAP_H

#include <android/bitmap.h>

class JniBitmap {
private:
    JNIEnv *env;
    jobject image;
public:
    AndroidBitmapInfo imageInfo;
    void *imagePixels;
    int ret;

    JniBitmap(JNIEnv *env, jobject image);

    ~JniBitmap();

    int getWidth() const { return imageInfo.width; };

    int getHeight() const { return imageInfo.height; };
};

#endif
