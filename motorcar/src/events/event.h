#ifndef EVENT_H
#define EVENT_H



namespace motorcar{
class Seat;
class Event
{
public:

    Event(Seat *seat);
    virtual ~Event(){}

    enum EventType{
        MOUSE,
        KEYBOARD,
        TOUCH,
        SIXDOF
    };

    virtual EventType type() const = 0;

    Seat *seat() const;

private:
    Seat *m_seat;
};

}

#endif // EVENT_H
