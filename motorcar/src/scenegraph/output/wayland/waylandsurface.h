#ifndef WAYLANDSURFACE_H
#define WAYLANDSURFACE_H
#include "../outputelement.h"
#include "glm/glm.hpp"
#include <GL/gl.h>

namespace motorcar{
class WaylandSurface : public OutputElement
{
public:
    enum SurfaceType{
        TOPLEVEL,
        TRANSIENT,
        POPUP,
        CURSOR,
        NA
    };

    WaylandSurface(SurfaceType type);
    virtual ~WaylandSurface(){}

    //get the texture handle for this surface
    virtual GLuint texture() = 0;
    //return the size of this surface in pixels
    virtual glm::ivec2 size() = 0;
    //do any per-frame setup required for drawing
    //note: this is the only safe place to change framebuffers
    virtual void prepare() = 0;
    //returns whether or not the surface is ready to draw
    virtual bool valid() = 0;

    enum MouseEvent{
        BUTTON_PRESS,
        BUTTON_RELEASE,
        MOVE
    };

    enum MouseButton{
        LEFT,
        RIGHT,
        MIDDLE,
        NONE

    };



    virtual void sendMouseEvent(MouseEvent eventType, MouseButton button, glm::vec2 localPostion) = 0;


    //virtual void sendMouseWheelEvent(Qt::Orientation orientation, int delta);
    

    glm::vec2 latestMouseEventPos() const;


    SurfaceType type() const;
    void setType(const SurfaceType &type);

protected:
    glm::vec2 m_latestMouseEventPos;
    SurfaceType m_type;

};
}


#endif // WAYLANDSURFACE_H
