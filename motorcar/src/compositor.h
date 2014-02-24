#ifndef COMPOSITOR_H
#define COMPOSITOR_H
#include "scenegraph/output/display/display.h"
#include "gl/openglcontext.h"

namespace motorcar {
class Compositor
{
public:
    virtual ~Compositor();



    virtual int start() = 0;

    virtual OpenGLContext *getContext() = 0;



    Display *display() const;
    void setDisplay(Display *display);

private:
    Display *m_display;
};
}


#endif // COMPOSITOR_H
