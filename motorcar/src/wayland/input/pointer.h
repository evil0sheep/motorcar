#ifndef POINTER_H
#define POINTER_H

#include "seat.h"

namespace motorcar{
class Pointer
{
public:
    Pointer(Seat *seat);

    glm::vec2 localPositon() const;
    void setLocalPositon(const glm::vec2 &localPositon);

    WaylandSurface *focus() const;
    void setFocus(WaylandSurface *focus);

private:
    Seat *m_seat;
    glm::vec2 m_localPositon;
    WaylandSurface *m_focus;
};
}


#endif // POINTER_H
