/****************************************************************************
**This file is part of the Motorcar 3D windowing framework
**
**
**Copyright (C) 2014 Forrest Reiling
**
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/

#include "qt/qtwaylandmotorcarseat.h"
#include "qt/qtwaylandmotorcarsurface.h"

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
    if(keyboardFocus != NULL){
        QtWaylandMotorcarSurface *surface = dynamic_cast<QtWaylandMotorcarSurface *>(keyboardFocus);
        if(surface){
            m_inputDevice->setKeyboardFocus(surface->surface());
        }else{
            m_inputDevice->setKeyboardFocus(NULL);
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
            m_inputDevice->setMouseFocus(surface->surface()->views().first(), localPos);
        }
    }
}
