#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "seat.h"

namespace motorcar{
class Keyboard
{
public:
    Keyboard(Seat *seat);

    WaylandSurface *focus() const;
    void setFocus(WaylandSurface *focus);

private:
    Seat *m_seat;
    WaylandSurface *m_focus;
};
}


#endif // KEYBOARD_H
