/****************************************************************************
**This file is part of the Motorcar 3D windowing framework
**
**
**Copyright (C) 2014 Forrest Reiling
**
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/
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




