/**
 //  GPUBasicDefinitions.h
 //  TVSGPULibrary
 // Android port (c) 2014 TrueVisionSolutions Pty Ltd.
 //  Created by Jesus Nuevo Chiquero TVS on 2/08/13.
 //  Copyright (c) 2013 TrueVision Solutions Pty Ltd. All rights reserved.
 //  Some constant values shader by vertex buffer objects and programs
*/

#ifndef TVSGPULibrary_GPUBasicDefinitions_h
#define TVSGPULibrary_GPUBasicDefinitions_h

#include <string>

#include <android/log.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#endif

#ifndef DEBUG
#define CHECK_GL_ERROR()
#define CHECK_EGL_ERROR()
#else
#define CHECK_GL_ERROR() { \
GLenum err = glGetError(); \
if (err != GL_NO_ERROR) \
  __android_log_print(ANDROID_LOG_ERROR, "GLerror: ", \
                      "GLError %x, at %s, line %d\n", \
                      err,__FILE__, __LINE__-1);      \
}
#define CHECK_EGL_ERROR() { \
EGLint err = eglGetError(); \
if (err != EGL_SUCCESS) { \
    __android_log_print(ANDROID_LOG_ERROR, "EGLError: ", \
                        "EGLError %d, at %s, line %d\n", \
                        err, __FILE__, __LINE__-1);      \
}                                                        \
}
#endif //DEBUG

namespace tvs {
namespace gpgpu {
#ifdef DEBUG
inline bool isCurrentProgram(GLuint prog, GLuint *curr = nullptr) {
  GLint currP;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currP);
  if (curr) *curr = currP;
  if(currP != (GLint)prog) 
    return false;
  return true;
}
#else
inline bool isCurrentProgram(GLuint) {
  return true;
}
#endif

//attribute locations for the VBO
enum kAttributeLocations {
  kPositionAttribute=0,
  kTex0CoordAttribute =1,
  kColorAttribute=1,
  kVertexNormalAttribute=2,
  kTex1CoordAttribute=kTex0CoordAttribute+1,
  kSingleFloatAttribute=4
};

static const GLfloat GPUIdentityMatrix[16] = {
  1,0,0,0,
  0,1,0,0,
  0,0,1,0,
  0,0,0,1};

const std::string kTexture0UniformName = "texture";
const std::string kProjMatrixUniformName = "matrix";
const std::string kVertexColorUniformName = "vertex_color";
const std::string kFilterStepUniformName = "filter_step";
const std::string kNormalMatrixUniformName = "normal_matrix";

} // namespace gpgpu
} // namespace tvs

#endif
