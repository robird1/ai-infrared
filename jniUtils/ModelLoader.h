#pragma once

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <math.h>
#include <iostream>
#include <android/asset_manager_jni.h>

using namespace std;

class ModelLoader {
public:
    ModelLoader();

    void initAssertManager(AAssetManager *pManager);

    int getAssertData(std::string &modelName);

private:
    AAssetManager *assertManager;
public:
    std::vector<unsigned char> m_vecStackCache;
};
