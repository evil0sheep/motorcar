#ifndef SIXDOFEVENT_H
#define SIXDOFEVENT_H

#include "mouseevent.h"
namespace motorcar{
class SixDofEvent : public MouseEvent
{
public:
    SixDofEvent(MouseEvent::Event event, MouseEvent::Button button, Seat *seat, glm::mat4 transform);
    glm::mat4 transform() const;

private:
    glm::mat4 m_transform;
};
}


#endif // SIXDOFEVENT_H
