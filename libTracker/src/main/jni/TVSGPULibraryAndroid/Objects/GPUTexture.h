/**
   GPUTexture: Texture classes with AndGraphicBuffer

   Support for floating point and mipmaps is missing from the Android
   version of the library.
   
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
*/
#ifndef __GPU_TEXTURE_H_
#define __GPU_TEXTURE_H_
#include <GLES2/gl2.h>

#include "GPUBasicDefinitions.h"
#include "AndGraphicBuffer.h"

namespace tvs {
namespace gpgpu {

class GPUTextureGraphicBuffer {
 public:
  GPUTextureGraphicBuffer();
  ~GPUTextureGraphicBuffer();

  /*
   * The texture may be initialised several times. Call destroy()
   * before init() explicitly if you want to destroy the texture.
   *
   * Reinitialisation is allowed because the texture may be destroyed
   * if the EGLContext gets destroyed. The underlying OpenGL texture
   * may not be valid, but the object may still live.
   */
  void init();
  void destroy();
  
  int width(){return _width;}
  int height(){return _height;}
  GLuint textureID() {return _textID;}

  bool bind(int textureUnit);
  void setFilteringParameters(GLenum interpMAG, GLenum interpMIN);
  void updateTexture(AndGraphicBuffer *gb);
  
  //Sets the graffic buffer to nullptr;
  void releaseAGB();
  
 private:
  GLuint _textID;
  int _width, _height;
  bool _init;

  AndGraphicBuffer *_gBuffPtr;
};

}  // namespace gpgpu
}  // namespace tvs

#endif
