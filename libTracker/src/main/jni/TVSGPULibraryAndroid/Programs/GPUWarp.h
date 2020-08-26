/**
   GPUWarp: Warping and conversion classes, with support for
   the EGL_IMAGE_EXTERNAL extension.
   
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
*/
#ifndef __GPU_WARP__
#define __GPU_WARP__

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <map>
#include <glm/glm.hpp>

namespace tvs {
namespace gpgpu {
class GPUWarp {
 public:
  //GPUWarp();
  virtual ~GPUWarp();

  //whether to the source texture will be OES_TEXTURE_EXTERNAL or not
  virtual bool init(bool useEGLExternal);
  
  void updateProjectionMatrix(const GLfloat * data);
  void updateProjectionMatrix(glm::mat4 mat);
  void updateTextureUnit(int textureUnit);
  virtual void updateScaling(float v=1.);
	
  virtual void use();
	
  virtual void draw(unsigned int elementCount, unsigned int elementOffset=0);
	
  virtual void label(const char*label, unsigned int length =0);
	
 protected:
  bool _init = false;
  GLuint _program;
  std::map<std::string, GLint> _uniformLocations;
  //shadow state
  int _textUnit; bool _textUnitInit = false;
};

//This class converts from BGRA to Gray, per pixel.
class GPUBGRA2Gray : public GPUWarp{
 public:
  bool init (bool useEGLExternal);
};

// Android doesn't support single channel GraphicBuffers, so use this
// class to pack a WxH grayscale image into a (W/4)xH color image.

class GPUBGRA2PackedGray : public GPUWarp {
 public:
  bool init(bool useEGLExternal);
  //normalised width/height of a pixel in the src texture.
  //Take rotations into account: if the image is upright, (1.0/text_width, 0.0f)
  //or (0.0f, 1.0/text_height) if it has to be rotated
  void updateSampleStep(float width, float height); 
};


class GPUGray2BGRA : public GPUWarp{
 public:
  bool init(bool /*useEGLExternal = false*/);
};

class GPURGBA2BGRA : public GPUWarp {
 public:
  bool init(bool useEGLExternal);
};

}  // namespace gpgpu
}  // namespace tvs

#endif // GPU_WARP_
