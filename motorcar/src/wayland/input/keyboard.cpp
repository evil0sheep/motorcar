#include "keyboard.h"

using namespace motorcar;
Keyboard::Keyboard(Seat *seat)
    :m_seat(seat)
    ,m_focus(NULL)
{
}
WaylandSurface *Keyboard::focus() const
{
    return m_focus;
}

void Keyboard::setFocus(WaylandSurface *focus)
{
    m_focus = focus;
}


