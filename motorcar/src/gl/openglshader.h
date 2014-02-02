#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H
#include <GL/gl.h>
#include <string>
#include <fstream>
#include <streambuf>
#include "GLSLHelper.h"




namespace motorcar{
class OpenGLShader
{
public:
    OpenGLShader(std::string &vertexShader, std::string &fragmentShader);
    OpenGLShader(std::ifstream &vertexShaderStream, std::ifstream &fragmentShaderStream);

    GLuint handle() const;



private:
    GLuint m_handle;
    GLuint compileShaderFromStrings(std::string &vertexShader, std::string &fragmentShader);
};
}


#endif // OPENGLSHADER_H
