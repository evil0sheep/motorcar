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

#ifndef QTWAYLANDMOTORCARSURFACE_H
#define QTWAYLANDMOTORCARSURFACE_H

#include "motorcar.h"
#include "qt/opengldata.h"
#include "qt/qtwaylandmotorcarcompositor.h"

#include <qwaylandsurface.h>
#include <qwaylandinput.h>

#include <QLinkedList>
#include <QtCompositor/qwaylandsurfaceview.h>

#include <glm/glm.hpp>

namespace qtmotorcar{

class QtWaylandMotorcarCompositor;

class QtWaylandMotorcarSurface : public motorcar::WaylandSurface
{
public:
    QtWaylandMotorcarSurface(QWaylandSurface *surface, QtWaylandMotorcarCompositor *compositor, motorcar::WaylandSurface::SurfaceType type);
    ~QtWaylandMotorcarSurface(){}

    //  inherited from WaylandSurface
    GLuint texture() override;
    //  Get the size of this surface in surface local coordinates
    glm::ivec2 size() override;
    //  Set the size of this surface in surface local coordinates
    void setSize(glm::ivec2 newSize) override;
    //  Get the position of this surface in parent surface-local coordinates
    glm::ivec2 position() override;
    //  return the parent surface
    WaylandSurface *parentSurface() override;

    void prepare() override;
    void sendEvent(const motorcar::Event &event) override;
    bool valid() override;

    QWaylandSurface *surface() const;
    void setSurface(QWaylandSurface *surface);

private:
    QWaylandSurface *m_surface;
    GLuint m_textureID;

    QtWaylandMotorcarCompositor *m_compositor;

    GLuint composeSurface(QWaylandSurface *surface, OpenGLData *glData);
    void paintChildren(QWaylandSurface *surface, QWaylandSurface *window, const QSize &windowSize, OpenGLData *glData);
    void computeSurfaceTransform(float ppcm);
};

}

#endif // QTWAYLANDMOTORCARSURFACE_H
