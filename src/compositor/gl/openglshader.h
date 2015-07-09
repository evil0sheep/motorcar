/****************************************************************************
**This file is part of the Motorcar 3D windowing framework
**
**
**Copyright (C) 2014 Forrest Reiling
**
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/

#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

//#include <GLES2/gl2.h>
#include <GL/gl.h>
#include <string>
#include <fstream>
#include <streambuf>





namespace motorcar {

#define SHADER_MOTORCARBARRELDISTORTION 0x01
#define SHADER_MOTORCARSURFACE 0x02
#define SHADER_DEPTHCOMPOSITEDSURFACE 0x03
#define SHADER_DEPTHCOMPOSITEDSURFACEBLITTER 0x04
#define SHADER_MOTORCARLINE 0x05
#define SHADER_SOFTKINETICDEPTHCAM 0x06

class OpenGLShader {
public:
  // OpenGLShader(std::string &vertexShader, std::string &fragmentShader);
  OpenGLShader(std::string vertexShaderFileName,
               std::string fragmentShaderFileName);
  OpenGLShader(int SHADER);
  GLuint handle() const;

private:
  GLuint m_handle;
  GLuint compileShaderFromStrings(std::string &vertexShader,
                                  std::string &fragmentShader);
};
}
#endif // OPENGLSHADER_H
