/**
   GPUWarp: Basic warp and color conversion classes, with support for
   the EGL_IMAGE_EXTERNAL extension.
   
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
*/
#include <android/log.h>

#include "GPUWarp.h"
#include "Utils/ShaderCompilation.h"
#include "Utils/utilities.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GPUBasicDefinitions.h"

const char kLogTag[] = "GPUWarp";
#define printInfo(...) __android_log_print(ANDROID_LOG_INFO, kLogTag, __VA_ARGS__)
#define printErr(...) __android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__)

using std::vector;
using std::string;

namespace tvs {
namespace gpgpu {

using internal::AttributeBinding;
using internal::UniformBinding;
using internal::glUtils;

GPUWarp::~GPUWarp () {
  if (_init) {
    glDeleteProgram(_program);
    CHECK_GL_ERROR();
    _program = 0;
    _init = false;
  }
}

bool GPUWarp::init(bool useEGLExternal) {
  //load shaders code
  std::string vertSrc = glUtils::loadShader("shaders/simpleTextureWarp.vsh");
  std::string fragSrc = glUtils::loadShader("shaders/simpleTextureWarp.fsh");
  if (useEGLExternal) {
    fragSrc = std::string("#define USE_EGL_IMAGE_EXTERNAL\n") + fragSrc;
  }
  vector<AttributeBinding> attributes = {
    AttributeBinding("position", kPositionAttribute),
    AttributeBinding("textureCoord", kTex0CoordAttribute)
  };
  vector<string> unif = {kProjMatrixUniformName, kTexture0UniformName, "scaling"};
  vector<UniformBinding> unifBind = internal::createBindingsForUniforms(unif);
  GLint status = internal::createProgram(vertSrc, fragSrc, attributes, unifBind, &_program);

  
  if (!status) {
    printErr("Program not created");
    return false;
  } else {
    _uniformLocations.insert(unifBind.begin(), unifBind.end());
    use();
    updateProjectionMatrix(glm::mat4());
    updateScaling(1.0f);
  }
  _init = true;
	
  return true;
}

void GPUWarp::label(const char *label, unsigned int length) {
  // not supported in some devices
}

void GPUWarp::use() {
#ifdef DEBUG
  if (isCurrentProgram(_program)) {
    printInfo("Reseting program to current value");
    return;
  }
#endif
  
  glUseProgram(_program);
  CHECK_GL_ERROR();
}

void GPUWarp::updateProjectionMatrix(const GLfloat * data) {
#ifdef DEBUG
  GLuint curr;
  if (!isCurrentProgram(_program, &curr)) {
    printErr("Current program is wrong! %s (current: %d, expected: %d)",
             __PRETTY_FUNCTION__, curr, _program);
    return;
  }
#endif
  glUniformMatrix4fv(_uniformLocations[kProjMatrixUniformName], 1, GL_FALSE, data);
  CHECK_GL_ERROR();
}

void GPUWarp::updateProjectionMatrix(glm::mat4 mat) {
#ifdef DEBUG
  if (!isCurrentProgram(_program)) {
    printInfo("Current program is wrong!");
    return;
  }
#endif
  updateProjectionMatrix(glm::value_ptr(mat));
}

void GPUWarp::updateTextureUnit(int textU) {
#ifdef DEBUG
  GLuint curr;
  if (!isCurrentProgram(_program, &curr)) {
    printErr("Current program is wrong! %s (current: %d, expected: %d)",
             __PRETTY_FUNCTION__, curr, _program);
    return;
  }
#endif
  glUniform1i(_uniformLocations[kTexture0UniformName],textU);
  CHECK_GL_ERROR();
}

void GPUWarp::updateScaling(float v) {
#ifdef DEBUG
  GLuint curr;
  if (!isCurrentProgram(_program, &curr)) {
    printErr("Current program is wrong! %s (current: %d, expected: %d)",
             __PRETTY_FUNCTION__, curr, _program);
    return;
  }
#endif
  glUniform1f(_uniformLocations["scaling"], v);
  CHECK_GL_ERROR();
}

void GPUWarp::draw(unsigned int count, unsigned int offset) {
#ifdef DEBUG
  GLuint curr;
  if (!isCurrentProgram(_program, &curr)) {
    printErr("Current program is wrong! %s (current: %d, expected: %d)",
             __PRETTY_FUNCTION__, curr, _program);
    return;
  }
#endif
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT,
                 BUFFER_OFFSET(offset*sizeof(GLuint)));	
  CHECK_GL_ERROR();
}

//******************************************************************
bool GPUBGRA2Gray::init(bool useEGLExternal) {
  //load shaders code	
  std::string vertSrc = glUtils::loadShader("shaders/simpleTextureWarp.vsh");
  std::string fragSrc = glUtils::loadShader("shaders/simpleTextureWarpBGRA2Gray.fsh");
  
  if (useEGLExternal) {
    fragSrc = std::string("#define USE_EGL_IMAGE_EXTERNAL\n") + fragSrc;
  }

  vector<AttributeBinding> attributes = {
    AttributeBinding("position", kPositionAttribute),
    AttributeBinding("textureCoord", kTex0CoordAttribute)
  };
  vector<string> unif = {kProjMatrixUniformName, kTexture0UniformName};
  vector<UniformBinding> unifBind = internal::createBindingsForUniforms(unif);
  GLint status = internal::createProgram(vertSrc, fragSrc, attributes, unifBind, &_program);

  if (!status) {
    printErr("Program not created, status: %d", status);
    return false;
  } else {
    _uniformLocations.insert(unifBind.begin(), unifBind.end());
    use();
    updateProjectionMatrix(glm::mat4());
  }
  _init = true;
  return true;
}
//******************************************************************

bool GPUBGRA2PackedGray::init(bool useEGLExternal) {
  std::string vertSrc = glUtils::loadShader("shaders/BGRA2PackedGray.vsh");
  std::string fragSrc = glUtils::loadShader("shaders/BGRA2PackedGray.fsh");
  
  if (useEGLExternal) {
    fragSrc = std::string("#define USE_EGL_IMAGE_EXTERNAL\n") + fragSrc;
  }

  //printInfo("Frag shader: %s", fragSrc.c_str());
  
  vector<AttributeBinding> attributes = {
    AttributeBinding("position", kPositionAttribute),
    AttributeBinding("textureCoord", kTex0CoordAttribute)
  };
  vector<string> unif = {kProjMatrixUniformName, kTexture0UniformName, "sampleStep"};
  vector<UniformBinding> unifBind = internal::createBindingsForUniforms(unif);
  GLint status = internal::createProgram(vertSrc, fragSrc, attributes, unifBind, &_program);

  if (!status) {
    printErr("Program not created, status: %d", status);
    return false;
  } else {
    _uniformLocations.insert(unifBind.begin(), unifBind.end());
    use();
    updateProjectionMatrix(glm::mat4());
  }
  CHECK_GL_ERROR();
  _init = true;
  
  return true;
}

void GPUBGRA2PackedGray::updateSampleStep(float width, float height) {
#ifdef DEBUG
  GLuint curr;
  if (!isCurrentProgram(_program, &curr)) {
    printErr("Current program is wrong! %s (current: %d, expected: %d)",
             __PRETTY_FUNCTION__, curr, _program);
    return;
  }
#endif
  glUniform2f(_uniformLocations["sampleStep"], width, height);
  CHECK_GL_ERROR();
}

//******************************************************************
bool GPUGray2BGRA::init(bool) {
  std::string vertSrc = glUtils::loadShader("shaders/simpleTextureWarp.vsh");
  std::string fragSrc = glUtils::loadShader("shaders/simpleTextureWarpGray2BGRA.fsh");

  vector<AttributeBinding> attributes = {
    AttributeBinding("position", kPositionAttribute),
    AttributeBinding("textureCoord", kTex0CoordAttribute)
  };
  vector<string> unif = {kProjMatrixUniformName, kTexture0UniformName, "scaling"};
  vector<UniformBinding> unifBind = internal::createBindingsForUniforms(unif);
  GLint status = internal::createProgram(vertSrc, fragSrc, attributes, unifBind, &_program);

  if (!status) {
    printErr("Program not created, status: %d", status);
    return false;
  } else {
    _uniformLocations.insert(unifBind.begin(), unifBind.end());
    use();
    updateProjectionMatrix(glm::mat4());
    updateScaling(1.0f);
  }
  _init = true;
	
  return true;
}

//******************************************************************
bool GPURGBA2BGRA::init(bool useEGLExternal) {
  std::string vertSrc = glUtils::loadShader("shaders/simpleTextureWarp.vsh");
  std::string fragSrc = glUtils::loadShader("shaders/simpleTextureWarpRGBA2BGRA.fsh");
  if (useEGLExternal) {
    fragSrc = std::string("#define USE_EGL_IMAGE_EXTERNAL\n") + fragSrc;
  }
  vector<AttributeBinding> attributes = {
    AttributeBinding("position", kPositionAttribute),
    AttributeBinding("textureCoord", kTex0CoordAttribute)
  };
  vector<string> unif = {kProjMatrixUniformName, kTexture0UniformName, "scaling"};
  vector<UniformBinding> unifBind = internal::createBindingsForUniforms(unif);
  GLint status = internal::createProgram(vertSrc, fragSrc, attributes, unifBind, &_program);

  if (!status) {
    printErr("Program not created");
    return false;
  } else {
    _uniformLocations.insert(unifBind.begin(), unifBind.end());
    use();
    updateProjectionMatrix(glm::mat4());
    updateScaling(1.0f);
  }
  _init = true;

  return true;
}

}
}
