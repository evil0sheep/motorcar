#ifndef OCULUSHMDCONTROLLER_H
#define OCULUSHMDCONTROLLER_H

#include "../scenegraph/output/display/rendertotexturedisplay.h"

namespace motorcar {
class OculusHMDController
{
public:
    OculusHMDController();
    Display *getDisplay(OpenGLContext *glContext, PhysicalNode &parent);

private:
    RenderToTextureDisplay *m_display;
};
}


#endif // OCULUSHMDCONTROLLER_H
