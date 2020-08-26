//
//  ShaderCompilation.h
//  TVSGPULibrary
//
//  Created by Jesus Nuevo Chiquero TVS on 13/08/13.
//  Copyright (c) 2013 TrueVision Solutions Pty Ltd. All rights reserved.
//  Android port (c) 2014 TrueVisionSolutions Pty Ltd.

#ifndef __TVSGPULibrary__ShaderCompilation__
#define __TVSGPULibrary__ShaderCompilation__

#include <vector>
#include <string>
#include <GLES2/gl2.h>

namespace tvs {
namespace gpgpu {
namespace internal {
typedef std::pair<std::string, GLint> AttributeBinding;
typedef AttributeBinding UniformBinding;

std::vector<UniformBinding> createBindingsForUniforms(std::vector<std::string>
                                                      unifNames);

GLint createProgram(std::string vertSrc, std::string fragSrc,
                    const std::vector<AttributeBinding> attributes,
                    std::vector<UniformBinding>& uniformsLocations,
                    GLuint *program);
}  // namespace internal
}  // namespace gpgpu
}  // namespace tvs

#endif /* defined(__TVSGPULibrary__ShaderCompilation__) */
