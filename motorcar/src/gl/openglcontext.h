#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H
#include <glm/glm.hpp>

namespace motorcar{
class OpenGLContext
{
public:
    virtual ~OpenGLContext(){}
    virtual glm::ivec2 defaultFramebufferSize() = 0;
    virtual void makeCurrent() = 0;

};
}


#endif // OPENGLCONTEXT_H
