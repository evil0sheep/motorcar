#include "waylandsurface.h"

using namespace motorcar;
WaylandSurface::WaylandSurface(SurfaceType type, bool isMotorcarSurface, ClippingMode clippingMode, bool depthCompositingEnabled)
    : m_type(type)
    , m_isMotorcarSurface(isMotorcarSurface)
    , m_clippingMode(clippingMode)
    , m_depthCompositingEnabled(depthCompositingEnabled)
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
WaylandSurface::ClippingMode WaylandSurface::clippingMode() const
{
    return m_clippingMode;
}

void WaylandSurface::setClippingMode(const WaylandSurface::ClippingMode &clippingMode)
{
    m_clippingMode = clippingMode;
}

bool WaylandSurface::depthCompositingEnabled() const
{
    return m_depthCompositingEnabled;
}

void WaylandSurface::setDepthCompositingEnabled(bool depthCompositingEnabled)
{
    m_depthCompositingEnabled = depthCompositingEnabled;
}

bool WaylandSurface::isMotorcarSurface() const
{
    return m_isMotorcarSurface;
}

void WaylandSurface::setIsMotorcarSurface(bool isMotorcarSurface)
{
    m_isMotorcarSurface = isMotorcarSurface;
}




