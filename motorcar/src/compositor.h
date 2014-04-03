#ifndef COMPOSITOR_H
#define COMPOSITOR_H
#include "scenegraph/output/display/display.h"
#include "gl/openglcontext.h"
#include "wayland-server.h"
#include "wayland-server-protocol.h"

namespace motorcar {
///This class handles only the invoking the draw calls on the scenegraph needed to display its contents
/*Classes implementing this interface are responsible for implementing the main draw loop of the application
 * and providing OpenGL contexts that Display classes can enable before drawing the scenegraph. */
class Compositor
{
public:
    virtual ~Compositor();

    ///starts the compositor main draw loop
    /*Starts the compositor and blocks until the compositor (usually another thread) has finished,
     * returning the exit code of the compositor thread*/
    virtual int start() = 0;

    ///gets the OpenGL context in which the compositing is taking place
    /*this context is used by display classes to allow them to do all of the drawing in the correct context.
     * eventually this should be capable of returning multiple contexts, or perhaps the contexts should be created externally and passed in*/
    virtual OpenGLContext *getContext() = 0;

//    void addDisplay(Display *display);
//    std::vector<Display *> displays() const;

    Display *display() const;
    void setDisplay(Display *display);

    virtual struct wl_display *wlDisplay() = 0;

private:
    //    std::vector<Display *> m_displays;
    Display *m_display;
};
}


#endif // COMPOSITOR_H
