#include <jni.h>
#include <string>
#include <iostream>"
#include "facecode_sh/facecode.h"

extern "C" {

extern "C" JNIEXPORT jstring JNICALL
Java_com_ulsee_facecode_Facecode_generateFacecode(
        JNIEnv *env,
        jobject /* this */,
        jfloatArray features) {
    float *array = env->GetFloatArrayElements(features, NULL);
    std::vector<float> feat{array, array + 256};

//    std::unique_ptr<ct::CodusEncoder> ct(new ct::CodusEncoder());
//    std::string facecode_base64;
//    std::vector<char> facecode_bin;
//    ct->to_base64(facecode_bin, &facecode_base64);
//    return env->NewStringUTF(facecode_base64.c_str());

    std::unique_ptr<fc::FaceCodeEncoder> fc(new fc::FaceCodeEncoder());
    fc->init(fc::Version::FC1a, fc::Device::ANY, feat, true);
    std::vector<char> facecode_bin;
    if (!fc->encode(&facecode_bin, fc::FaceCodeEncoder::MODE::RSA_NO)) {
        std::cout << "Error encoding FaceCode" << std::endl;
        exit(-1);
    }
    std::string facecode_base64;
    fc->to_base64(facecode_bin, &facecode_base64);

    return env->NewStringUTF(facecode_base64.c_str());
}
}