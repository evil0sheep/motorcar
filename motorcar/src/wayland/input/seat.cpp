#include "seat.h"
#include "keyboard.h"
#include "pointer.h"

using namespace motorcar;
Seat::Seat()
    :m_keyboard(new Keyboard(this))
    ,m_pointer(new Pointer(this))
{
}

Seat::~Seat()
{
    delete m_keyboard;
    delete m_pointer;
}
Keyboard *Seat::keyboard() const
{
    return m_keyboard;
}

void Seat::setKeyboard(Keyboard *keyboard)
{
    m_keyboard = keyboard;
}
Pointer *Seat::pointer() const
{
    return m_pointer;
}

void Seat::setPointer(Pointer *pointer)
{
    m_pointer = pointer;
}
WaylandSurface *Seat::pointerFocus() const
{
    return m_pointer->focus();
}

void Seat::setPointerFocus(WaylandSurface *pointerFocus, glm::vec2 localPosition)
{
    m_pointer->setFocus(pointerFocus);
    m_pointer->setLocalPositon(localPosition);

    m_keyboard->setFocus(pointerFocus);
}
WaylandSurface *Seat::keyboardFocus() const
{
    return m_keyboard->focus();
}

void Seat::setKeyboardFocus(WaylandSurface *keyboardFocus)
{
    m_keyboard->setFocus(keyboardFocus);
}

void Seat::ensureKeyboardFocusIsValid(WaylandSurface *oldSurface, WaylandSurface *nextSurface)
{
    if (m_keyboard->focus() == oldSurface || m_keyboard->focus() == NULL){
            setKeyboardFocus(nextSurface);
    }
}




