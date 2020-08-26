#include <cstring>
#include "ModelLoader.h"
#include "Log.h"

using namespace std;

ModelLoader::ModelLoader() {

}

void ModelLoader::initAssertManager(AAssetManager *pManager) {
    this->assertManager = pManager;
}

int ModelLoader::getAssertData(std::string &modelName) {
    LOGD("getAssertData",modelName.c_str(),"");
    AAsset *asset = AAssetManager_open(assertManager, modelName.c_str(), AASSET_MODE_BUFFER);
    int size = AAsset_getLength(asset);
    const char *buffer = (const char *) AAsset_getBuffer(asset);
    m_vecStackCache.resize(size);
    memcpy(m_vecStackCache.data(), buffer, size);
    AAsset_close(asset);
    return size;
}


