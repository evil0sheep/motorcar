#ifndef WAYLANDSURFACE_H
#define WAYLANDSURFACE_H
#include "../outputelement.h"
#include "glm/glm.hpp"
#include <GL/gl.h>

namespace motorcar{
class WaylandSurface : public OutputElement
{
public:
    WaylandSurface();
    virtual ~WaylandSurface(){}

    //get the texture handle for this surface
    virtual GLuint texture() = 0;
    //return the size of this surface in pixels
    virtual glm::ivec2 size() = 0;
    //do any per-frame setup required for drawing
    //note: this is the only safe place to change framebuffers
    virtual void prepare() = 0;

private:

};
}


#endif // WAYLANDSURFACE_H
