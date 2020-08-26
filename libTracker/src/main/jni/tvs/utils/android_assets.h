#ifndef __ANDROID_ASSETS_H__
#define __ANDROID_ASSETS_H__
#include <string>
#include <vector>

#include <android/log.h>
#include <android/asset_manager.h>

bool copyFileFromAsset(AAssetManager* mgr, const std::string& file,
                       const std::string& tmpFile);

#endif
