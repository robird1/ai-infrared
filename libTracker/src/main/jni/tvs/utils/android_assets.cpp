#include "android_assets.h"
#include <memory>

const char kLogTag[] = "UlsTracker";

#define printErr(...) {__android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__);}

//==============================================================================
bool copyFileFromAsset(AAssetManager* mgr, const std::string& file,
                                   const std::string& tmpFile) {
  AAsset *ass = AAssetManager_open(mgr, file.c_str(), AASSET_MODE_UNKNOWN);
  if (!ass) {
    printErr("Error reading file %s", file.c_str());
    return false;
  }
  off_t length = AAsset_getLength(ass);
  std::unique_ptr<char []> data (new char[length+1]);
  int readData = AAsset_read(ass, data.get(), length);
  if (readData != length) {
    printErr("Error reading data from asset %s", file.c_str());
    return false;
  }
  FILE* tmpF = fopen(tmpFile.c_str(), "wb");
  if (!tmpF) {
    printErr("Error opening file: %s", tmpFile.c_str());
    return false;
  }
  int written = fwrite(data.get(), sizeof(char), length, tmpF);
  if (written != length) {
    printErr("Error writing tmp file for asset %s. Written: %d, expected: %ld",
             file.c_str(), written, length);
    return false;
  }
  fclose(tmpF);
  return true;
}
