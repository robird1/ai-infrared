//
//  ShaderCompilation.cpp
//  TVSGPULibrary
//
//  Created by Jesus Nuevo Chiquero TVS on 13/08/13.
//  Copyright (c) 2013 TrueVision Solutions Pty Ltd. All rights reserved.
//  Android port (c) 2014 TrueVisionSolutions Pty Ltd.

#include "ShaderCompilation.h"

#include <assert.h>
#include <android/log.h>

#include "GPUBasicDefinitions.h"

using std::vector;
using std::string;

const char kLogTag[] = "ShaderCompilation";
#define printErr(...) __android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__)


namespace tvs {
namespace gpgpu {
namespace internal {

std::vector<UniformBinding> createBindingsForUniforms(std::vector<std::string>
                                                      unifNames) {
  std::vector<UniformBinding> unifs;
  for(size_t i=0; i<unifNames.size(); i++){
    unifs.push_back(UniformBinding(unifNames.at(i),0));
  }
  return unifs;
}

namespace {
GLint compileShader(GLenum target, std::string src, GLuint *shader) {
  GLint status;
	
  *shader = glCreateShader(target);
	
  const GLchar *str = src.c_str();
  glShaderSource(*shader, 1, &str, NULL);
  glCompileShader(*shader);
  CHECK_GL_ERROR();
#ifdef DEBUG
  GLint length =0;
  glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &length);
  if(length>0){
    GLchar *log = (GLchar*) malloc(length);
    glGetShaderInfoLog(*shader, length, &length, log);
    printErr("Shader compilation log:\n%s", log);
    free(log);
  }
#endif
	
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
  if(!status){
    printErr("Failed to compile shader:\n%s", src.c_str());
  }
  CHECK_GL_ERROR();
  return status;
}

GLint linkProgram(GLuint program) {
  GLint status;
  glLinkProgram(program);
#ifdef DEBUG
  GLint length =0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  if(length>0){
    GLchar *log = (GLchar*) malloc(length);
    glGetProgramInfoLog(program, length, &length, log);
    printErr("Program linking log (size: %d):\n%s", length, log);
    free(log);
  }
#endif
	
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if(!status){
    printErr("Failed to link program %d\n", program);
  }
  CHECK_GL_ERROR();
  return status;
}

GLint validateProgram(GLuint program) {
  glValidateProgram(program);
#ifdef DEBUG
  GLint length =0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  if(length>0){
    GLchar *log = (GLchar*) malloc(length);
    glGetProgramInfoLog(program, length, &length, log);
    printErr("Program validate log (size: %d):\n%s", length, log);
    free(log);
  }
#endif

  GLint status;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
  if(!status)
    printErr("Program validation failed for program: %d", program);
  CHECK_GL_ERROR();
  return status;
}
} // namespace

GLint createProgram(const std::string vertSrc, const std::string fragSrc,
                    const vector<std::pair<string, GLint> > attributes,
                    vector<std::pair<string, GLint> > &uniformsLocations,
                    GLuint *program) {
  GLuint vertSh = 0, fragSh = 0, prog = 0, status = 1;
	
  prog = glCreateProgram();
	
  status *= compileShader(GL_VERTEX_SHADER, vertSrc, &vertSh);
  status *= compileShader(GL_FRAGMENT_SHADER, fragSrc, &fragSh);
  CHECK_GL_ERROR();
	
  glAttachShader(prog, vertSh);
  CHECK_GL_ERROR();
  glAttachShader(prog, fragSh);
  CHECK_GL_ERROR();
	
  //Bind attributes
  for(size_t i = 0; i < attributes.size(); i++){
    if(attributes.at(i).first.length()){
      const GLchar *name = attributes.at(i).first.c_str();
      GLint loc = attributes.at(i).second;
      glBindAttribLocation(prog, loc, name);
    }
    CHECK_GL_ERROR();
  }
	
  status *= linkProgram(prog);
	
#ifdef DEBUG
  status *= validateProgram(prog);
#endif
	
  //location of uniforms
  if (status) {
    for (size_t i = 0; i < uniformsLocations.size(); i++) {
      if (uniformsLocations.at(i).first.length()) {
        GLint loc = glGetUniformLocation(prog,
                                         uniformsLocations.at(i).first.c_str());
        CHECK_GL_ERROR();
        if (loc < 0) printErr("Unknown uniform '%s'\n",
                             uniformsLocations.at(i).first.c_str());
        assert(loc>=0 && "Unknown uniform");
        uniformsLocations.at(i).second = loc;
      }
    }
    *program = prog;
  }
	
  if (vertSh) {
    glDeleteShader(vertSh);
    glDetachShader(prog, vertSh);
  }
  if (fragSh) {
    glDeleteShader(fragSh);
    glDetachShader(prog, fragSh);
  }
  CHECK_GL_ERROR();
  return status;
}

}  // namespace internal
}  // namespace gpgpu
}  // namespace tvs
