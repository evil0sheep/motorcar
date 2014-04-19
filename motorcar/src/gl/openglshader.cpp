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


