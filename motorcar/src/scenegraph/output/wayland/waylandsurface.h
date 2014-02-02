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

    virtual GLuint texture() = 0;
    virtual glm::ivec2 size() = 0;

private:

};
}


#endif // WAYLANDSURFACE_H
