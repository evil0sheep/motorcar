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

#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H
#include <events/event.h>
#include <glm/glm.hpp>

namespace motorcar{
class MouseEvent : public Event
{
public:
    enum Event{
        BUTTON_PRESS,
        BUTTON_RELEASE,
        MOVE,
        ENTER,
        LEAVE
    };

    enum Button{
        LEFT,
        RIGHT,
        MIDDLE,
        NONE

    };

    MouseEvent(MouseEvent::Event event, MouseEvent::Button button, glm::vec2 localPostion, Seat *seat);
    EventType type() const override;



    MouseEvent::Event event() const;
    MouseEvent::Button button() const;
    glm::vec2 localPosition() const;

private:
    MouseEvent::Event m_event;
    MouseEvent::Button m_button;
    glm::vec2 m_localPosition;

};
}


#endif // MOUSEEVENT_H
