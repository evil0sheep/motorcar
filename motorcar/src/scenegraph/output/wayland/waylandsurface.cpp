#include "waylandsurface.h"

using namespace motorcar;
WaylandSurface::WaylandSurface(SurfaceType type)
    : m_type(type)
{

}

glm::vec2 WaylandSurface::latestMouseEventPos() const
{
    return m_latestMouseEventPos;
}


WaylandSurface::SurfaceType WaylandSurface::type() const
{
    return m_type;
}

void WaylandSurface::setType(const SurfaceType &type)
{
    m_type = type;
}
