#include "qtwaylandmotorcarseat.h"
#include "qtwaylandmotorcarsurface.h"
#include <QtCompositor/qwaylandinput.h>

using namespace qtmotorcar;
QtWaylandMotorcarSeat::QtWaylandMotorcarSeat(QWaylandInputDevice *inputDevice)
    :m_inputDevice(inputDevice)
{
}
QWaylandInputDevice *QtWaylandMotorcarSeat::inputDevice() const
{
    return m_inputDevice;
}

void QtWaylandMotorcarSeat::setInputDevice(QWaylandInputDevice *inputDevice)
{
    m_inputDevice = inputDevice;
}

void QtWaylandMotorcarSeat::setKeyboardFocus(motorcar::WaylandSurface *keyboardFocus)
{
    motorcar::Seat::setKeyboardFocus(keyboardFocus);
    if(keyboardFocus == NULL){
        m_inputDevice->setKeyboardFocus(NULL);
    }else{
        QtWaylandMotorcarSurface *surface = dynamic_cast<QtWaylandMotorcarSurface *>(keyboardFocus);
        if(surface){
            m_inputDevice->setKeyboardFocus(surface->surface());
        }
    }

}

void QtWaylandMotorcarSeat::setPointerFocus(motorcar::WaylandSurface *pointerFocus, glm::vec2 localPosition)
{
    motorcar::Seat::setPointerFocus(pointerFocus, localPosition);
    QPointF localPos(localPosition.x, localPosition.y);
    if(pointerFocus == NULL){
        m_inputDevice->setMouseFocus(NULL, localPos);
    }else{
        QtWaylandMotorcarSurface *surface = dynamic_cast<QtWaylandMotorcarSurface *>(pointerFocus);
        if(surface){
            m_inputDevice->setMouseFocus(surface->surface(), localPos);
        }
    }
}

