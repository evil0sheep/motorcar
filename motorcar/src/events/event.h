#ifndef EVENT_H
#define EVENT_H

namespace motorcar{
class Event
{
public:

    Event();
    virtual ~Event(){}

    enum EventType{
        MOUSE,
        KEYBOARD,
        TOUCH
    };

    virtual EventType type() const = 0;
};

}

#endif // EVENT_H
