/**
 //  GPUVertexData.cpp
 //  TVSGPULibrary
 //  Android port (c) 2014 TrueVisionSolutions Pty Ltd.
 //  Created by Jesus Nuevo Chiquero TVS on 14/08/13.
 //  Copyright (c) 2013 TrueVision Solutions Pty Ltd. All rights reserved.
 */

#include "GPUVertexData.h"
#include <assert.h>
#include <dlfcn.h>
#include <iostream>
#include <limits>
#include <string.h>
#include <sstream>

#include <android/log.h>

#include "GPUBasicDefinitions.h"

#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "GPUVertexData";
#define printInfo(...) __android_log_print(ANDROID_LOG_INFO, kLogTag, __VA_ARGS__)
#else
const char kLogTag[] = "UlsFaceTracker";
#define printInfo(...)
#endif

#define printErr(...) __android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__)

/**
 * support for label_EXT is optional
 *
 * support for GL_OES_vertex_array_object is optional if you only use
 * GPUVertexData of type VD_TEXTCOORDS - VBO's are used then
 */
static class VAOFunctions {
 public:
  VAOFunctions() {
    _init = false;
    bindVertexArrayOES = nullptr;
    deleteVertexArraysOES = nullptr;
    genVertexArraysOES = nullptr;
    isVertexArrayOES = nullptr;
    labelObjectEXT = nullptr;
  }

  bool supportsVAO() {
    if (_init) return _supportsVAO;
    initialise();
    return _supportsVAO;
  }
  bool hasLabelObjectEXT() {
    if (_init) return _hasLabelObjectExt;
    initialise();
    return _hasLabelObjectExt;
  }

  // these prototypes defined in gl2ext.h
  PFNGLBINDVERTEXARRAYOESPROC bindVertexArrayOES;
  PFNGLDELETEVERTEXARRAYSOESPROC deleteVertexArraysOES;
  PFNGLGENVERTEXARRAYSOESPROC genVertexArraysOES;
  PFNGLISVERTEXARRAYOESPROC isVertexArrayOES;

  typedef void (*myPFNGLLABELOBJECTEXTPROC)(GLenum type, GLuint object,
                                            GLsizei length,
                                            const GLchar *label);
  myPFNGLLABELOBJECTEXTPROC labelObjectEXT;
  GLenum VERTEX_ARRAY_OBJECT_EXT = 0x9154;

 private:
  void initialise() {
    parseExtensions();
    initFunctions();
    _init = true;
  }
  void parseExtensions() {
    const char * extensions = (const char *) glGetString(GL_EXTENSIONS);

    std::string item;
    std::string str (extensions);
    std::istringstream iss (str);

    while (iss >> item) {
      if (item == "GL_OES_vertex_array_object")
        _supportsVAO = true;
      else if (item == "GL_EXT_debug_label")
        _hasLabelObjectExt = true;
    }
  }

  void initFunctions() {
    //get dl handle
    void *handle = dlopen("libGLESv2.so", RTLD_LAZY);

    if (_supportsVAO) {
      bindVertexArrayOES =
          (PFNGLBINDVERTEXARRAYOESPROC) dlsym(handle, "glBindVertexArrayOES");
      deleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSOESPROC)
          dlsym(handle, "glDeleteVertexArraysOES");
      genVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC)
          dlsym(handle, "glGenVertexArraysOES");
      isVertexArrayOES = (PFNGLISVERTEXARRAYOESPROC)dlsym(handle,
                                                          "glIsVertexArrayOES");
      if (!bindVertexArrayOES || !deleteVertexArraysOES || !genVertexArraysOES
          || !isVertexArrayOES) {
        printErr("Some VAO functions are missing!");
        assert(0);
        _supportsVAO = false;
        return;
      }
    }

    if (_hasLabelObjectExt) {
      labelObjectEXT = (myPFNGLLABELOBJECTEXTPROC) dlsym(handle,
                                                         "glLabelObjectEXT");
      if (!labelObjectEXT) _hasLabelObjectExt = false;
    }
  }

  bool _init;
  bool _hasLabelObjectExt;
  bool _supportsVAO;
} sVAOSupport;

//*****************************************************************************/
//*****************************************************************************/

namespace tvs{
namespace gpgpu{

GPUVertexData::GPUVertexData(kVertexDataType type) {
  _type = type;

  _indexCount = 0;
  _vertexCount = 0;
  _offsetSubmitCount = 0;
  _vertSubmitCount = 0;

  _vao = 0;

  if(_type == VD_TEXTCOORDS) _dataSize = 5;
  else if(_type == VD_COLOR) _dataSize = 7;
  else if(_type == VD_COLOR_RADIUS) _dataSize = 8;
  else if(_type == VD_TEXTCOORDS_NORMALS) _dataSize = 8;
  else if(_type == VD_NORMALS) _dataSize = 6;
  else if(_type == VD_TEXTCOORDS_TEXTCOORDS) _dataSize = 7;
  else if(_type == VD_TEXTCOORDS_FLOAT) _dataSize = 6;
  else if(_type == VD_TEXTCOORDS_TEXTCOORDS_FLOAT) _dataSize = 8;
  else if(_type == VD_NORMALS_FLOAT) _dataSize = 7;

  if (sVAOSupport.supportsVAO()) {
    sVAOSupport.genVertexArraysOES(1, &_vao);
    sVAOSupport.bindVertexArrayOES(_vao);

    //printInfo("created VAO with name: %d", _vao);
    CHECK_GL_ERROR();
    initialiseAoS();
  } else {
    if (type != VD_TEXTCOORDS) {
      printErr("Unsupported type of VAO with VBO emulation");
      abort();
    }
    glGenBuffers(1, &_vertexBuffer);
    glGenBuffers(1, &_elementsBuffer);
    CHECK_GL_ERROR();
  }
}
//*****************************************************************************/

void GPUVertexData::initialiseAoS() {
  glGenBuffers(1, &_vertexBuffer);
  CHECK_GL_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
  glVertexAttribPointer(kPositionAttribute, 3, GL_FLOAT, GL_FALSE,
                        _dataSize*sizeof(GLfloat), BUFFER_OFFSET(0));
  glEnableVertexAttribArray(kPositionAttribute);
  CHECK_GL_ERROR();

  if(_type == VD_TEXTCOORDS || _type == VD_TEXTCOORDS_NORMALS
     || _type == VD_TEXTCOORDS_FLOAT || _type == VD_TEXTCOORDS_TEXTCOORDS
     || _type == VD_TEXTCOORDS_TEXTCOORDS_FLOAT){
    glVertexAttribPointer(kTex0CoordAttribute, 2, GL_FLOAT, GL_FALSE,
                          _dataSize*sizeof(GLfloat),
                          BUFFER_OFFSET(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(kTex0CoordAttribute);
    CHECK_GL_ERROR();

    if (_type == VD_TEXTCOORDS_NORMALS) {
      glVertexAttribPointer(kVertexNormalAttribute, 3, GL_FLOAT, GL_FALSE,
                            _dataSize*sizeof(GLfloat),
                            BUFFER_OFFSET(5*sizeof(GLfloat)));
      glEnableVertexAttribArray(kVertexNormalAttribute);
      CHECK_GL_ERROR();
    } else if (_type == VD_TEXTCOORDS_FLOAT) {
      glVertexAttribPointer(kSingleFloatAttribute, 1, GL_FLOAT, GL_FALSE,
                            _dataSize*sizeof(GLfloat),
                            BUFFER_OFFSET(5*sizeof(GLfloat)));
      glEnableVertexAttribArray(kSingleFloatAttribute);
      CHECK_GL_ERROR();
    } else if (_type == VD_TEXTCOORDS_TEXTCOORDS
               || _type == VD_TEXTCOORDS_TEXTCOORDS_FLOAT){
      glVertexAttribPointer(kTex1CoordAttribute, 2, GL_FLOAT, GL_FALSE,
                            _dataSize*sizeof(GLfloat),
                            BUFFER_OFFSET(5*sizeof(GLfloat)));
      glEnableVertexAttribArray(kTex1CoordAttribute);
      CHECK_GL_ERROR();

      if(_type == VD_TEXTCOORDS_TEXTCOORDS_FLOAT) {
        glVertexAttribPointer(kSingleFloatAttribute, 1, GL_FLOAT, GL_FALSE,
                              _dataSize*sizeof(GLfloat),
                              BUFFER_OFFSET(7*sizeof(GLfloat)));
        glEnableVertexAttribArray(kSingleFloatAttribute);
        CHECK_GL_ERROR();
      }
    }
  }
  else if(_type == VD_COLOR || _type == VD_COLOR_RADIUS){
    glVertexAttribPointer(kColorAttribute, 4, GL_FLOAT, GL_FALSE,
                          _dataSize*sizeof(GLfloat),
                          BUFFER_OFFSET(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(kColorAttribute);
    if(_type == VD_COLOR_RADIUS){
      glVertexAttribPointer(kSingleFloatAttribute, 1, GL_FLOAT, GL_FALSE,
                            _dataSize*sizeof(GLfloat),
                            BUFFER_OFFSET(7*sizeof(GLfloat)));
      glEnableVertexAttribArray(kSingleFloatAttribute);
    }
  }
  else if(_type == VD_NORMALS || _type == VD_NORMALS_FLOAT){
    glVertexAttribPointer(kVertexNormalAttribute, 3, GL_FLOAT, GL_FALSE,
                          _dataSize*sizeof(GLfloat),
                          BUFFER_OFFSET(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(kVertexNormalAttribute);
    if(_type == VD_NORMALS_FLOAT){
      glVertexAttribPointer(kSingleFloatAttribute, 1, GL_FLOAT, GL_FALSE,
                            _dataSize*sizeof(GLfloat),
                            BUFFER_OFFSET(6*sizeof(GLfloat)));
      glEnableVertexAttribArray(kSingleFloatAttribute);
    }
  }
  else{
    assert(0 && "Unknow type of vertex data");
  }

  glGenBuffers(1, &_elementsBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementsBuffer);
  CHECK_GL_ERROR();
}

//*****************************************************************************/

GPUVertexData::~GPUVertexData() {
  unbind();

  glDisableVertexAttribArray(kPositionAttribute);
  CHECK_GL_ERROR();

  {
    if(_type == VD_TEXTCOORDS)
      glDisableVertexAttribArray(kTex0CoordAttribute);
    else if(_type == VD_COLOR)
      glDisableVertexAttribArray(kColorAttribute);
    else if(_type == VD_COLOR_RADIUS){
      glDisableVertexAttribArray(kColorAttribute);
      glDisableVertexAttribArray(kSingleFloatAttribute);
    }
    else if(_type == VD_TEXTCOORDS_NORMALS){
      glDisableVertexAttribArray(kTex0CoordAttribute);
      glDisableVertexAttribArray(kVertexNormalAttribute);
    }
    else if(_type == VD_NORMALS)
      glDisableVertexAttribArray(kVertexNormalAttribute);
    else if(_type == VD_TEXTCOORDS_TEXTCOORDS){
      glDisableVertexAttribArray(kTex0CoordAttribute);
      glDisableVertexAttribArray(kTex1CoordAttribute);
    }
    else if(_type == VD_TEXTCOORDS_FLOAT){
      glDisableVertexAttribArray(kTex0CoordAttribute);
      glDisableVertexAttribArray(kSingleFloatAttribute);
    }
    else if(_type == VD_TEXTCOORDS_TEXTCOORDS_FLOAT){
      glDisableVertexAttribArray(kTex0CoordAttribute);
      glDisableVertexAttribArray(kTex1CoordAttribute);
      glDisableVertexAttribArray(kSingleFloatAttribute);
    }
    else if(_type == VD_NORMALS_FLOAT){
      glDisableVertexAttribArray(kVertexNormalAttribute);
      glDisableVertexAttribArray(kSingleFloatAttribute);
    }
    CHECK_GL_ERROR();

    glDeleteBuffers(1, &_vertexBuffer);
    CHECK_GL_ERROR();
  }

  glDeleteBuffers(1, &_elementsBuffer);
  if (sVAOSupport.supportsVAO())
    sVAOSupport.deleteVertexArraysOES(1, &_vao);
  CHECK_GL_ERROR();

  // printInfo("Deleted VAO: %d", _vao);
  _vao = 0;

}

//*****************************************************************************/
void GPUVertexData::label(const char * str) {
  if (sVAOSupport.hasLabelObjectEXT()) {
    GLsizei len = static_cast<GLsizei>(std::min(32u, static_cast<unsigned int>(strlen(str))));

    sVAOSupport.labelObjectEXT(sVAOSupport.VERTEX_ARRAY_OBJECT_EXT, _vao, len,
                               str);
    CHECK_GL_ERROR();
  }
}
//*****************************************************************************/

void GPUVertexData::bind() {
  if (sVAOSupport.supportsVAO()) {
#ifdef DEBUG
    GLint p;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING_OES, &p);
    if(p == _vao) {
      printInfo("Setting current GPUVertexData");
      return;
    }
    //printInfo("VAO object: %d - current %d", _vao, p);
#endif
    sVAOSupport.bindVertexArrayOES(_vao);
    CHECK_GL_ERROR();
  } else {
    if (_type == VD_TEXTCOORDS) {
      glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
      glVertexAttribPointer(kPositionAttribute, 3, GL_FLOAT, GL_FALSE,
                            _dataSize*sizeof(GLfloat), BUFFER_OFFSET(0));
      glEnableVertexAttribArray(kPositionAttribute);
      glVertexAttribPointer(kTex0CoordAttribute, 2, GL_FLOAT, GL_FALSE,
                            _dataSize*sizeof(GLfloat),
                            BUFFER_OFFSET(3*sizeof(GLfloat)));
      glEnableVertexAttribArray(kTex0CoordAttribute);
      CHECK_GL_ERROR();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementsBuffer);
      CHECK_GL_ERROR();
    } else {
      printErr("This VAO type is not supported with VBO emulation");
      abort();
    }
  }
}

//*****************************************************************************/

void GPUVertexData::unbind() {
  if (sVAOSupport.supportsVAO()) {
    sVAOSupport.bindVertexArrayOES(0);
    CHECK_GL_ERROR();
  } else {
    if (_type == VD_TEXTCOORDS) {
      glDisableVertexAttribArray(kPositionAttribute);
      glDisableVertexAttribArray(kTex0CoordAttribute);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
  }
}

//*****************************************************************************/

void GPUVertexData::setVertexCount(unsigned int n) {
  if(_vertexCount == n) return;

  _vertexCount = n;

  //allocate buffer, set parameters
  //if(_vertexData) delete [] _vertexData;

 _vertexData.resize(_dataSize*_vertexCount);

  const GLenum use = GL_STREAM_DRAW;
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, _dataSize*sizeof(GLfloat)*_vertexCount, NULL,
               use);
  CHECK_GL_ERROR();
}

void GPUVertexData::setElementIndicesCount(unsigned int n) {
  if(n == _indexCount) return;
  _indexCount = n;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementsBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, n*sizeof(GLushort), NULL,
               GL_STATIC_DRAW);
  CHECK_GL_ERROR();
}

void GPUVertexData::updateElementIndices(const GLushort * indices,
                                         unsigned int count,
                                         unsigned int offset) {
  assert(count+offset <= _indexCount && "Bad indices count");
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementsBuffer);
  if (count == _indexCount) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount*sizeof(GLushort), indices,
                 GL_STATIC_DRAW);
  } else {
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset*sizeof(GLushort),
                    count*sizeof(GLushort), indices);
  }
}

void GPUVertexData::updateVertexPosition(const GLfloat* position,
                                         unsigned int count,
                                         unsigned int offset, bool submit) {
  assert(count+offset <= _vertexCount && "Bad vertex count");

  for (unsigned int j=offset; j<count+offset; j++) {
    _vertexData[_dataSize*j] = position[3*(j-offset)];
    _vertexData[_dataSize*j+1] = position[3*(j-offset)+1];
    _vertexData[_dataSize*j+2] = position[3*(j-offset)+2];

    unsigned int k;
    if(_vertSubmitCount > 0) k = _offsetSubmitCount + _vertSubmitCount;
    else k = _vertSubmitCount;
    _offsetSubmitCount = std::min(_offsetSubmitCount, offset);
    _vertSubmitCount = std::max(k - _offsetSubmitCount,
                                offset + count - _offsetSubmitCount);
    if(submit) submitData();
  }
}

//*****************************************************************************/

void GPUVertexData::updateTextureCoordinates(const GLfloat * texcoord,
                                             unsigned int count,
                                             unsigned int offset, bool submit) {
  assert(offset+count <= _vertexCount && "Bad vertex count");

  assert(_type == VD_TEXTCOORDS || _type == VD_TEXTCOORDS_FLOAT
         || _type == VD_TEXTCOORDS_NORMALS || _type == VD_TEXTCOORDS_TEXTCOORDS
         || _type == VD_TEXTCOORDS_TEXTCOORDS_FLOAT
         && "GPUVertexData doesn't have a texture coordinate");

  //copy the data to the _vertexData array and submit
  if(_type != VD_TEXTCOORDS_TEXTCOORDS
     && _type != VD_TEXTCOORDS_TEXTCOORDS_FLOAT){
    for (unsigned int j=offset; j<count+offset; j++) {
      _vertexData[_dataSize*j + 3] = texcoord[2*(j-offset)];
      _vertexData[_dataSize*j + 4] = texcoord[2*(j-offset) +1];
    }
  }
  else{
    for (unsigned int j=offset; j<count+offset; j++) {
      _vertexData[_dataSize*j + 3] = texcoord[4*(j-offset)];
      _vertexData[_dataSize*j + 4] = texcoord[4*(j-offset) +1];
      _vertexData[_dataSize*j + 5] = texcoord[4*(j-offset) +2];
      _vertexData[_dataSize*j + 6] = texcoord[4*(j-offset) +3];
    }
  }
  unsigned int k;
  if(_vertSubmitCount > 0) k = _offsetSubmitCount + _vertSubmitCount;
  else k = _vertSubmitCount;
  _offsetSubmitCount = std::min(_offsetSubmitCount, offset);
  _vertSubmitCount = std::max(k - _offsetSubmitCount,
                              offset + count - _offsetSubmitCount);

  if(submit) submitData();
}

void GPUVertexData::updateVertexNormals(const GLfloat *normals,
                                        unsigned int count,
                                        unsigned int offset, bool submit) {
  assert(offset+count <= _vertexCount && "Bad vertex count");

  assert(_type == VD_NORMALS || _type == VD_NORMALS_FLOAT
         || _type == VD_TEXTCOORDS_NORMALS);
  int exoff;
  if(_type == VD_NORMALS || _type == VD_NORMALS_FLOAT) exoff = 3;
  else exoff = 5;

  for (unsigned int j=0; j<count+offset; j++) {
    _vertexData[_dataSize*j + exoff] = normals[3*(j-offset)];
    _vertexData[_dataSize*j + exoff+1] = normals[3*(j-offset)+1];
    _vertexData[_dataSize*j + exoff+2] = normals[3*(j-offset)+2];
  }
  unsigned int k;
  if(_vertSubmitCount > 0) k = _offsetSubmitCount + _vertSubmitCount;
  else k = _vertSubmitCount;
  _offsetSubmitCount = std::min(_offsetSubmitCount, offset);
  _vertSubmitCount = std::max(k - _offsetSubmitCount,
                              offset + count - _offsetSubmitCount);

  if(submit) submitData();
}

void GPUVertexData::updateVertexColor(const GLfloat *color, unsigned int count,
                                      unsigned int offset, bool submit) {
  assert(offset+count <= _vertexCount && "Bad vertex count");

  assert(_type == VD_COLOR || _type == VD_COLOR_RADIUS);

  for (unsigned int j=0; j<count+offset; j++) {
    _vertexData[_dataSize*j+3] = color[4*(j-offset)];
    _vertexData[_dataSize*j+4] = color[4*(j-offset)+1];
    _vertexData[_dataSize*j+5] = color[4*(j-offset)+2];
    _vertexData[_dataSize*j+6] = color[4*(j-offset)+3];
  }
  unsigned int k;
  if(_vertSubmitCount > 0) k = _offsetSubmitCount + _vertSubmitCount;
  else k = _vertSubmitCount;
  _offsetSubmitCount = std::min(_offsetSubmitCount, offset);
  _vertSubmitCount = std::max(k - _offsetSubmitCount,
                              offset + count - _offsetSubmitCount);

  if(submit) submitData();
}

void GPUVertexData::setVertexColorConstant(const GLfloat * rgba,
                                           unsigned int count,
                                           unsigned int offset, bool submit) {
  assert(offset+count <= _vertexCount && "Bad vertex count");

  assert(_type == VD_COLOR || _type == VD_COLOR_RADIUS);

  for (unsigned int j=offset; j<count+offset; j++) {
    _vertexData[_dataSize*j+3] = rgba[0];
    _vertexData[_dataSize*j+4] = rgba[1];
    _vertexData[_dataSize*j+5] = rgba[2];
    _vertexData[_dataSize*j+6] = rgba[3];
  }

  unsigned int k;
  if(_vertSubmitCount > 0) k = _offsetSubmitCount + _vertSubmitCount;
  else k = _vertSubmitCount;
  _offsetSubmitCount = std::min(_offsetSubmitCount, offset);
  _vertSubmitCount = std::max(k - _offsetSubmitCount,
                              offset + count - _offsetSubmitCount);

  if(submit) submitData();
}

void GPUVertexData::updateSingleFloatAttribute(const GLfloat *data,
                                               unsigned int count,
                                               unsigned int offset,
                                               bool submit) {
  assert(count+offset <= _vertexCount);

  assert(_type == VD_NORMALS_FLOAT || _type == VD_TEXTCOORDS_FLOAT
         || _type == VD_TEXTCOORDS_TEXTCOORDS_FLOAT
         || _type == VD_COLOR_RADIUS);

  int exoff = 7;
  if(_type == VD_COLOR_RADIUS) exoff = 7;
  else if(_type == VD_NORMALS_FLOAT) exoff = 6;
  else if(_type == VD_TEXTCOORDS_FLOAT) exoff = 5;
  else if(_type == VD_TEXTCOORDS_TEXTCOORDS_FLOAT) exoff = 7;
  else assert( 0 && "bad VertexData type");
  for (unsigned int j=offset; j<count+offset; j++) {
    _vertexData[j*_dataSize + exoff] = data[j-offset];
  }

  unsigned int k;
  if(_vertSubmitCount > 0) k = _offsetSubmitCount + _vertSubmitCount;
  else k = _vertSubmitCount;
  _offsetSubmitCount = std::min(_offsetSubmitCount, offset);
  _vertSubmitCount = std::max(k - _offsetSubmitCount,
                              offset + count - _offsetSubmitCount);
  if(submit) submitData();
}

void GPUVertexData::submitData() {
  if(_vertSubmitCount == 0) return;
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

  if (_vertSubmitCount >= .3*_vertexCount || _vertSubmitCount <= 512) {
    glBufferData(GL_ARRAY_BUFFER, _dataSize*sizeof(GLfloat)*_vertexCount,
                 _vertexData.data(), GL_STREAM_DRAW);
  } else {
    glBufferSubData(GL_ARRAY_BUFFER,
                    _dataSize*sizeof(GLfloat)*_offsetSubmitCount,
                    _dataSize*sizeof(GLfloat)*_vertSubmitCount,
                    _vertexData.data()+_dataSize*(_offsetSubmitCount));
  }

  CHECK_GL_ERROR();
  _offsetSubmitCount = std::numeric_limits<unsigned int>::max();
  _vertSubmitCount = 0;
}

//*****************************************************************************/
//*****************************************************************************/
//*****************************************************************************/

//#pragma mark - Utility functions

GPUVertexData * createBackgroundQuad() {
  GPUVertexData *vao = createBackgroundQuad(0, 0, 1, 1);
  return vao;
}

GPUVertexData * createBackgroundQuad(GLfloat x0, GLfloat y0,
                                     GLfloat width, GLfloat height) {
  GPUVertexData *vao = new GPUVertexData(GPUVertexData::VD_TEXTCOORDS);
  GLfloat vertices[] = {
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, 1.0f
  };
  GLushort indices[] = {
    2,1,0,
    0,3,2
  };
  GLfloat texcoord[] = {
    x0, y0,
    x0 + width, y0,
    x0 + width, y0 + height,
    x0, y0 + height
  };

  vao->setTriangleCount(2);
  vao->setVertexCount(4);
  vao->updateElementIndices(indices, 6, 0);
  vao->updateVertexPosition(vertices, 4, 0, false);
  vao->updateTextureCoordinates(texcoord, 4, 0, true);
  return vao;
}
}  // namespace gpggpu
}  // namespace tvs
