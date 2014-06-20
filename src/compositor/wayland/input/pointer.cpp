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
#include <wayland/input/pointer.h>

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




