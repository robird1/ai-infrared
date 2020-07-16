/**
 *  GPUVertexData.h
 *  Android port of TVSGPULibrary
 *  (c) 2014 TrueVisionSolutions Pty. Ltd.
 *  Created by Jesus Nuevo Chiquero TVS on 14/08/13.
 *  Copyright (c) 2013 TrueVision Solutions Pty Ltd. All rights reserved.
*/

#ifndef __TVSGPULibrary__GPUVertexData__
#define __TVSGPULibrary__GPUVertexData__

#include <iostream>
#include <vector>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace tvs{
namespace gpgpu{
//All vertex array objects have vertex coordinates

class GPUVertexData {
 public:
  enum kVertexDataType {
    VD_TEXTCOORDS = 0,
    VD_COLOR,
    VD_COLOR_RADIUS,
    VD_TEXTCOORDS_NORMALS,
    VD_NORMALS,
    VD_TEXTCOORDS_TEXTCOORDS,
    VD_TEXTCOORDS_FLOAT,
    VD_TEXTCOORDS_TEXTCOORDS_FLOAT,
    VD_NORMALS_FLOAT
  };

  GPUVertexData(kVertexDataType type);
  ~GPUVertexData();

  void label(const char * str); // Not supported in all devices 
  
  void bind();
  void unbind();
  
  unsigned int getVertexCount(){ return _vertexCount;}
  unsigned int getElementIndicesCount() {return _indexCount;}
  kVertexDataType getType() { return _type;}
  unsigned int getVertexDataSize() {return _dataSize;}
	
  void setVertexCount(unsigned int n);
  void setElementIndicesCount(unsigned int n);
  void setTriangleCount(unsigned int n) {setElementIndicesCount(3*n);}
	
  void updateElementIndices(const GLushort * indices, unsigned int count,
                            unsigned int offset);

  void updateVertexPosition(const GLfloat* position, unsigned int count,
                            unsigned int offset, bool submit);
  //if the GPUVertexData has 2 texture coordinates, the texture
  //coordinates are interleaved as in [s0, t0, s1, t1]_0, [s0, t0, s1,
  //t1]_1, ...
  void updateTextureCoordinates(const GLfloat * texcoord, unsigned int count,
                                unsigned int offset, bool submit);
  void updateVertexColor(const GLfloat *color, unsigned int count,
                         unsigned int offset, bool submit);
  void updateVertexNormals(const GLfloat *normals, unsigned int count,
                           unsigned int offset, bool submit);
  void setVertexColorConstant(const GLfloat * rgba, unsigned int count,
                              unsigned int offset, bool submit);
	
  void updateSingleFloatAttribute(const GLfloat *data, unsigned int count,
                                  unsigned int offset, bool submit);
	
  void submitData();

 private:
  void initialiseAoS();

  GLuint _vao;
  GLuint _vertexBuffer;
  GLuint _elementsBuffer;

  std::vector<GLfloat> _vertexData;

  unsigned int _indexCount, _vertexCount;
  unsigned int _offsetSubmitCount, _vertSubmitCount;

  unsigned int _dataSize;//structure size (in floats)

  kVertexDataType _type;
};

//utility functions to create a quad with default texture coordinates
GPUVertexData * createBackgroundQuad();
//create a quad spanning from -1 to 1, with given texture coordinates
GPUVertexData * createBackgroundQuad(GLfloat x0, GLfloat y0,
                                     GLfloat width, GLfloat height);
} // namespace gpgpu
} // namespace tvs

#endif /* defined(__TVSGPULibrary__GPUVertexData__) */
