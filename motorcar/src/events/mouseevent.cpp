#include "mouseevent.h"

using namespace motorcar;



MouseEvent::MouseEvent(MouseEvent::Event event, MouseEvent::Button button, glm::vec2 localPostion)
    :m_event(event)
    ,m_button(button)
    ,m_localPosition(localPostion)
{
}

Event::EventType MouseEvent::type() const
{
    return EventType::MOUSE;
}
MouseEvent::Event MouseEvent::event() const
{
    return m_event;
}
MouseEvent::Button MouseEvent::button() const
{
    return m_button;
}
glm::vec2 MouseEvent::localPosition() const
{
    return m_localPosition;
}





