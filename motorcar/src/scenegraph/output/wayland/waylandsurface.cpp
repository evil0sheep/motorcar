#include "waylandsurface.h"

using namespace motorcar;
WaylandSurface::WaylandSurface()
{

}

glm::vec2 WaylandSurface::latestMouseEventPos() const
{
    return m_latestMouseEventPos;
}

