#include "pointer.h"

using namespace motorcar;
Pointer::Pointer(Seat *seat)
    :m_seat(seat)
    ,m_focus(NULL)
{
}
glm::vec2 Pointer::localPositon() const
{
    return m_localPositon;
}

void Pointer::setLocalPositon(const glm::vec2 &localPositon)
{
    m_localPositon = localPositon;
}
WaylandSurface *Pointer::focus() const
{
    return m_focus;
}

void Pointer::setFocus(WaylandSurface *focus)
{
    m_focus = focus;
}


