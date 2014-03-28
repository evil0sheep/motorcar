#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H
#include "event.h"
#include <stdint.h>

namespace motorcar{
class KeyboardEvent : public Event
{
public:
    enum Event{
        KEY_PRESS,
        KEY_RELEASE
    };

    KeyboardEvent(KeyboardEvent::Event event, uint32_t keyCode, Seat *seat);
    EventType type() const override;
    KeyboardEvent::Event event() const;
    uint32_t keyCode() const;


private:
    KeyboardEvent::Event m_event;
    uint32_t m_keyCode;
};
}


#endif // KEYBOARDEVENT_H
