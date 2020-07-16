/**
   utlities: basic functionalities to load shaders from a folder
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
*/
#include "utilities.h"
#include <android/log.h>
#include <fstream>

#ifdef TVS_INTERNAL_DEBUGGING
#define printErr(...) __android_log_print(ANDROID_LOG_ERROR, "GPUUtils", __VA_ARGS__)
#define printInfo(...) __android_log_print(ANDROID_LOG_INFO, "GPUUtils", __VA_ARGS__)
#else
#define printErr(...)
#define printInfo(...)
#endif

using std::string;
namespace tvs {
namespace gpgpu {
namespace internal {
std::string glUtils::_shaderPath = "";
std::map<string, string> glUtils::_shaderCode = std::map<string, string>();

std::string glUtils::loadShader(std::string filename) {
  if (_shaderCode.size() == 0) loadShaderStrings();

  printInfo("Shader count: %d", _shaderCode.size());

  std::map<string, string>::iterator it = _shaderCode.find(filename);
  if (it != _shaderCode.end()) {
    return it->second;
  }

  if (_shaderPath.length() == 0) return std::string();

  std::string path = _shaderPath+"/"+filename;
  
  std::ifstream fn;		
  fn.open(path.c_str(), std::ifstream::in);
  if(!fn.is_open()){
    printErr("Error loading shader from %s", path.c_str());
    return std::string();
  }
  std::string data = std::string((std::istreambuf_iterator<char>(fn)), std::istreambuf_iterator<char>());
  fn.close();
  return data;
}


void glUtils::loadShaderStrings() {
  printInfo("Loading shader strings");
  _shaderCode.insert(std::make_pair<std::string, std::string> (
      "shaders/BGRA2PackedGray.fsh",
      "#ifdef USE_EGL_IMAGE_EXTERNAL\n"
      "#extension GL_OES_EGL_image_external : require\n"
      "uniform samplerExternalOES texture;\n"
      "#else\n"
      "uniform sampler2D texture;\n"
      "#endif\n"
      "\n"
      "precision highp float;\n"
      "uniform highp float scaling;\n"
      "\n"
      "varying vec2 coordinate[4];\n"
      "varying vec2 tcoord;\n"
      "\n"
      "const mediump vec4 cvt = vec4(0.299,0.587,0.114,0);\n"
      "\n"
      "void main() {\n"
      "  mediump mat4 v;\n"
      "	\n"
      "  v[0] = texture2D(texture, coordinate[0]);\n"
      "  v[1] = texture2D(texture, coordinate[1]);\n"
      "  v[2] = texture2D(texture, coordinate[2]);\n"
      "  v[3] = texture2D(texture, coordinate[3]);\n"
      "  gl_FragColor = cvt*v;\n"
      "}\n"));
  
  _shaderCode.insert(std::make_pair<std::string, std::string> (
      "shaders/BGRA2PackedGray.vsh",
      "precision highp float;\n"
      "\n"
      "uniform mat4 matrix;\n"
      "attribute vec4 position;\n"
      "attribute vec2 textureCoord;\n"
      "uniform vec2 sampleStep;\n"
      "\n"
      "varying vec2 coordinate[4];\n"
      "varying vec2 tcoord;\n"
      "\n"
      "void main() {\n"
      "	gl_Position = matrix * position;\n"
      "	\n"
      "	vec2 step15 = 2.0*sampleStep;\n"
      "	vec2 step05 = 1.0*sampleStep;\n"
      "	\n"
      "	tcoord = textureCoord;\n"
      "	coordinate[0] = textureCoord - step15;\n"
      "	coordinate[1] = textureCoord - step05;\n"
      "	coordinate[2] = textureCoord + step05;\n"
      "	coordinate[3] = textureCoord + step15;\n"
      "}"));  
  _shaderCode.insert(std::make_pair<std::string, std::string> (
      "shaders/meshDrawer.fsh",
      "varying lowp vec4 destination_color;\n"
      "void main(void) { \n"
      "   gl_FragColor = destination_color; \n"
      "}"));
  _shaderCode.insert(std::make_pair<std::string, std::string> (
      "shaders/meshDrawer.vsh",
      "precision highp float;\n"
      "uniform mat4 matrix;\n"
      "\n"
      "attribute vec3 position;\n"
      "attribute vec4 point_color;\n"
      "\n"
      "varying lowp vec4 destination_color;\n"
      "\n"
      "void main(void) {\n"
      "    gl_Position = matrix*vec4(position, 1.0);\n"
      "    destination_color = point_color;\n"
      "}"));
  
  _shaderCode.insert(std::make_pair<std::string, std::string> (
      "shaders/simpleTextureWarp.fsh",
      "#ifdef USE_EGL_IMAGE_EXTERNAL\n"
      "#extension GL_OES_EGL_image_external : require\n"
      "uniform samplerExternalOES texture;\n"
      "#else\n"
      "uniform sampler2D texture;\n"
      "#endif\n"
      "\n"
      "precision highp float;\n"
      "varying vec2 coordinate;\n"
      "uniform highp float scaling;\n"
      "\n"
      "void main() {\n"
      "	highp vec4 v = texture2D(texture, coordinate);\n"
      "	gl_FragColor = vec4(vec3(scaling),1.)*v;\n"
      "}"));
  
  _shaderCode.insert(std::make_pair<std::string, std::string> (
      "shaders/simpleTextureWarp.vsh",
      "precision highp float;\n"
      "\n"
      "uniform mat4 matrix;\n"
      "attribute vec4 position;\n"
      "attribute vec2 textureCoord;\n"
      "varying vec2 coordinate;\n"
      "\n"
      "void main() {\n"
      "	gl_Position = matrix * position;\n"
      "	coordinate = textureCoord;\n"
      "}\n"
      ""));
  _shaderCode.insert(std::make_pair<std::string, std::string> (
      "shaders/simpleTextureWarpBGRA2Gray.fsh",
      "#ifdef USE_EGL_IMAGE_EXTERNAL\n"
      "#extension GL_OES_EGL_image_external : require\n"
      "uniform samplerExternalOES texture;\n"
      "#else\n"
      "uniform sampler2D texture;\n"
      "#endif\n"
      "\n"
      "precision highp float;\n"
      "varying  vec2 coordinate;\n"
      "\n"
      "const highp vec4 cvt = vec4(0.299,0.587,0.114,0);\n"
      "\n"
      "void main() {\n"
      "	gl_FragColor = vec4(vec3(dot(cvt,texture2D(texture,coordinate))),1);\n"
      "}\n"
      ""));
  _shaderCode.insert(std::make_pair<std::string, std::string> (
      "shaders/simpleTextureWarpGray2BGRA.fsh",
      "varying mediump vec2 coordinate;\n"
      "uniform sampler2D textr;\n"
      "uniform lowp float scaling;\n"
      "\n"
      "void main() {\n"
      "	lowp float r = scaling*texture2D(textr, coordinate).r;\n"
      "	gl_FragColor = vec4(r,r,r,1.);\n"
      "}" ));

  _shaderCode.insert(std::make_pair<std::string, std::string> (
      "shaders/simpleTextureWarpRGBA2BGRA.fsh",
      "#ifdef USE_EGL_IMAGE_EXTERNAL\n"
      "#extension GL_OES_EGL_image_external : require\n"
      "uniform samplerExternalOES textr;\n"
      "#else\n"
      "uniform sampler2D textr;\n"
      "#endif\n"
      "\n"
      "precision highp float;\n"
      "varying vec2 coordinate;\n"
      "uniform highp float scaling;\n"
      "\n"
      "void main() {\n"
      "	highp vec4 v = texture2D(textr,coordinate);\n"
      "	gl_FragColor = vec4(vec3(scaling),1.)*v.bgra;\n"
      "}" ));
}

}  // namespace internal
}  // namespcae gpgpu
}  // namespace tvs
