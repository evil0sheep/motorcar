#include "waylandsurface.h"

using namespace motorcar;
WaylandSurface::WaylandSurface(SurfaceType type)
    : m_type(type)
{

}



WaylandSurface::SurfaceType WaylandSurface::type() const
{
    return m_type;
}

void WaylandSurface::setType(const SurfaceType &type)
{
    m_type = type;
}
