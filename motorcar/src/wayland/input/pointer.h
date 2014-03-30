#ifndef POINTER_H
#define POINTER_H


#include "seat.h"

namespace motorcar{
class WaylandSurfaceNode;
class Pointer
{
public:
    Pointer(Seat *seat);

    glm::vec2 localPositon() const;
    void setLocalPositon(const glm::vec2 &localPositon);

    WaylandSurface *focus() const;
    void setFocus(WaylandSurface *focus);

    WaylandSurfaceNode *cursorNode() const;
    void setCursorNode(WaylandSurfaceNode *cursorNode);

    glm::ivec2 cursorHotspot() const;
    void setCursorHotspot(const glm::ivec2 &cursorHotspot);

private:
    Seat *m_seat;
    glm::vec2 m_localPositon;
    WaylandSurface *m_focus;
    WaylandSurfaceNode * m_cursorNode;
    glm::ivec2 m_cursorHotspot;
};
}


#endif // POINTER_H
