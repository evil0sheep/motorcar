#include "sixdofevent.h"
using namespace motorcar;
SixDofEvent::SixDofEvent(Event event, Button button, Seat *seat, glm::mat4 transform)
    :MouseEvent(event, button, glm::vec2(0), seat)
    ,m_transform(transform)
{
}
glm::mat4 SixDofEvent::transform() const
{
    return m_transform;
}



