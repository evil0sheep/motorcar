#include "event.h"
#include "../wayland/input/seat.h"

using namespace motorcar;
Event::Event(Seat *seat)
    :m_seat(seat)
{

}
Seat *Event::seat() const
{
    return m_seat;
}



