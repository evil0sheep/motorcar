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
#include "openglshader.h"


using namespace motorcar;

//OpenGLShader::OpenGLShader(std::string &vertexShader, std::string &fragmentShader)
//{
//    compileShaderFromStrings(vertexShader, fragmentShader);
//}

OpenGLShader::OpenGLShader(std::string vertexShaderFileName, std::string fragmentShaderFileName)
{
    std::ifstream vertexShaderStream(vertexShaderFileName), fragmentShaderStream(fragmentShaderFileName) ;
    std::string vertexShader, fragmentShader;

    //intitialize
    vertexShaderStream.seekg(0, std::ios::end);
    vertexShader.reserve(vertexShaderStream.tellg());
    vertexShaderStream.seekg(0, std::ios::beg);

    fragmentShaderStream.seekg(0, std::ios::end);
    fragmentShader.reserve(fragmentShaderStream.tellg());
    fragmentShaderStream.seekg(0, std::ios::beg);

    vertexShader.assign((std::istreambuf_iterator<char>(vertexShaderStream)),
                     std::istreambuf_iterator<char>());

    fragmentShader.assign((std::istreambuf_iterator<char>(fragmentShaderStream)),
                     std::istreambuf_iterator<char>());

    //std::cout << vertexShader << std::endl << fragmentShader << std::endl;

    vertexShaderStream.close();
    fragmentShaderStream.close();

    m_handle = compileShaderFromStrings(vertexShader, fragmentShader);

}

GLuint OpenGLShader::handle() const
{
    return m_handle;
}

GLuint OpenGLShader::compileShaderFromStrings(std::string &vertexShader, std::string &fragmentShader)
{
    GLuint VS; //handles to vert shader object
    GLuint FS; //handles to frag shader object
    GLint vCompiled, fCompiled, linked; //status of shader


    //create a program object and attach the compiled shader
    m_handle = glCreateProgram();

    if(vertexShader.length() > 0){


        VS = glCreateShader(GL_VERTEX_SHADER);
        const char *vs_c_str = vertexShader.c_str();
        glShaderSource(VS, 1, &vs_c_str, NULL);
        //compile shader and print log
        glCompileShader(VS);
        /* check shader status requires helper functions */
        printOpenGLError();
        glGetShaderiv(VS, GL_COMPILE_STATUS, &vCompiled);
        printShaderInfoLog(VS);

        if (!vCompiled) {
                printf("Error compiling vertex shader:\n\n ");
                return 0;
        }
        glAttachShader(m_handle, VS);
    }

    if(fragmentShader.length() > 0){

        FS = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fs_c_str = fragmentShader.c_str();
        glShaderSource(FS, 1, &fs_c_str, NULL);
        //compile shader and print log
        glCompileShader(FS);
        /* check shader status requires helper functions */
        printOpenGLError();
        glGetShaderiv(FS, GL_COMPILE_STATUS, &fCompiled);
        printShaderInfoLog(FS);

        if (!fCompiled) {
                printf("Error compiling fragment shader:\n\n ");
                return 0;
        }
        glAttachShader(m_handle, FS);

    }

    glLinkProgram(m_handle);
    /* check shader status requires helper functions */
    printOpenGLError();
    glGetProgramiv(m_handle, GL_LINK_STATUS, &linked);
    if (!linked) {
    printf("Error linking shader: %d\n\n", m_handle);
    return 0;
    }


    //printProgramInfoLog(m_handle);

    return m_handle;
}


