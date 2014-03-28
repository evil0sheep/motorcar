#ifndef SEAT_H
#define SEAT_H

#include "../output/waylandsurface.h"

namespace motorcar{
class Keyboard;
class Pointer;
class Seat
{
public:
    Seat();
    virtual ~Seat();

    Keyboard *keyboard() const;
    void setKeyboard(Keyboard *keyboard);

    WaylandSurface *keyboardFocus() const;
    virtual void setKeyboardFocus(WaylandSurface *keyboardFocus);

    Pointer *pointer() const;
    void setPointer(Pointer *pointer);

    WaylandSurface *pointerFocus() const;
    virtual void setPointerFocus(WaylandSurface *pointerFocus, glm::vec2 localPosition);

    ///Guaruntees that the current keyboard focus is as valid possible
    /*If oldsurface is the current keyboard focus, or if the current keyboard focus is NULL, this sets the kyeboard focus to be nextsurface*/
    void ensureKeyboardFocusIsValid(WaylandSurface *oldSurface, WaylandSurface *nextSurface);

private:
    Keyboard *m_keyboard;
    Pointer *m_pointer;
};
}


#endif // SEAT_H
