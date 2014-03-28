#include "keyboardevent.h"

using namespace motorcar;


KeyboardEvent::KeyboardEvent(KeyboardEvent::Event event, uint32_t keyCode)
    :m_event(event)
    ,m_keyCode(keyCode)
{

}

Event::EventType KeyboardEvent::type() const
{
    return EventType::KEYBOARD;
}
KeyboardEvent::Event KeyboardEvent::event() const
{
    return m_event;
}
uint32_t KeyboardEvent::keyCode() const
{
    return m_keyCode;
}



