#ifndef WAYLANDSURFACE_H
#define WAYLANDSURFACE_H
#include "../outputelement.h"
#include "glm/glm.hpp"

namespace motorcar{
class WaylandSurface : public OutputElement
{
public:
    WaylandSurface();
    virtual ~WaylandSurface();

    virtual GLuint texture() = 0;
    virtual glm::ivec2 size() = 0;
};
}


#endif // WAYLANDSURFACE_H
