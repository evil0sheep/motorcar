#ifndef QTWAYLANDMOTORCARSEAT_H
#define QTWAYLANDMOTORCARSEAT_H


#include "qtwaylandmotorcarcompositor.h"

namespace qtmotorcar{
class QtWaylandMotorcarSeat : public motorcar::Seat
{
public:
    QtWaylandMotorcarSeat(QWaylandInputDevice *inputDevice);

    QWaylandInputDevice *inputDevice() const;
    void setInputDevice(QWaylandInputDevice *inputDevice);

    void setKeyboardFocus(motorcar::WaylandSurface *keyboardFocus) override;
    void setPointerFocus(motorcar::WaylandSurface *pointerFocus, glm::vec2 localPosition) override;

private:
    QWaylandInputDevice *m_inputDevice;
};
}


#endif // QTWAYLANDMOTORCARSEAT_H
