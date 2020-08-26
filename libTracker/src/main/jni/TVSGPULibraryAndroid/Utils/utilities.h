/**
   utlities: basic functionalities to load shaders from a folder
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
*/
#ifndef __GL_UTILITIES_H_
#define __GL_UTILITIES_H_

#include <string>
#include <map>

namespace tvs {
namespace gpgpu {
namespace internal {
class glUtils{
 public:
  //filename is "shaders/<shadername>"
  static std::string loadShader(std::string filename);
  
  static std::string _shaderPath;
  static std::map<std::string, std::string> _shaderCode;

 private:
  static void loadShaderStrings();
};

}  // namespace internal
}  // namespace gpgpu
}  // namespace tvs

#endif
