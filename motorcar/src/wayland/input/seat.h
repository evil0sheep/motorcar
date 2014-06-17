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
