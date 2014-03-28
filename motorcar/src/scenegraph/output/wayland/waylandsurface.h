#ifndef WAYLANDSURFACE_H
#define WAYLANDSURFACE_H
#include "../outputelement.h"
#include "glm/glm.hpp"
#include <GLES2/gl2.h>
//#include <GL/gl.h>
#include "../../../events/events.h"

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

    ///Get the texture handle for this surface
    virtual GLuint texture() = 0;
    ///Get the size of this surface in pixels
    virtual glm::ivec2 size() = 0;
    ///Get the position of this surface in parent surface-local coordinates
    virtual glm::ivec2 position() = 0;
    ///return the parent surface
    virtual WaylandSurface *parentSurface() = 0;
    ///do any per-frame setup required for drawing
    /*note: this is the only safe place to change framebuffers*/
    virtual void prepare() = 0;
    ///returns whether or not the surface is ready to draw
    virtual bool valid() = 0;



    virtual void sendEvent(const Event &event) = 0;


    //virtual void sendMouseWheelEvent(Qt::Orientation orientation, int delta);
    


    SurfaceType type() const;
    void setType(const SurfaceType &type);

protected:
    SurfaceType m_type;

};
}


#endif // WAYLANDSURFACE_H
