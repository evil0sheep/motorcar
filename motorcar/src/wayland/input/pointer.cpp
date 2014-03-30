#include "pointer.h"

using namespace motorcar;
Pointer::Pointer(Seat *seat)
    :m_seat(seat)
    ,m_focus(NULL)
    ,m_cursorNode(NULL)
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
WaylandSurfaceNode *Pointer::cursorNode() const
{
    return m_cursorNode;
}

void Pointer::setCursorNode(WaylandSurfaceNode *cursorNode)
{
    m_cursorNode = cursorNode;
}
glm::ivec2 Pointer::cursorHotspot() const
{
    return m_cursorHotspot;
}

void Pointer::setCursorHotspot(const glm::ivec2 &cursorHotspot)
{
    m_cursorHotspot = cursorHotspot;
}




