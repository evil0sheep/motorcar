#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H
#include "event.h"
#include "glm/glm.hpp"

namespace motorcar{
class MouseEvent : public Event
{
public:
    enum Event{
        BUTTON_PRESS,
        BUTTON_RELEASE,
        MOVE
    };

    enum Button{
        LEFT,
        RIGHT,
        MIDDLE,
        NONE

    };

    MouseEvent(MouseEvent::Event event, MouseEvent::Button button, glm::vec2 localPostion);
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
