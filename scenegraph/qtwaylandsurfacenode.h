#ifndef WAYLANDSURFACENODE_H
#define WAYLANDSURFACENODE_H
/****************************************************************************
**This file is part of the MotorCar QtWayland Compositor
**
**
**Copyright (C) 2013 Forrest Reiling
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

#include "scenegraphnode.h"
#include "qwaylandsurface.h"
#include <QtDebug>
#include "glm/gtc/matrix_inverse.hpp"


class QtwaylandSurfaceNode : public SceneGraphNode
{
public:
    QtwaylandSurfaceNode(QObject *parent, QWaylandSurface *surface, glm::mat4 transform = glm::mat4(1));
    virtual ~QtwaylandSurfaceNode();

    QWaylandSurface *surface() const;
    void setSurface(QWaylandSurface *surface);
    //returns the transform which maps normalized surface coordinates to the local node space
    glm::mat4 surfaceTransform() const;



    //takes a ray in the local Node space and returns whether or not the ray insersects the plane of this surface;
    // t: the ray's intersection distance to the surface
    // localIntersection : the ray's intersection with the surface in wayland "surface local coordinates" as a QPoint for use with QTWayland
    bool computeLocalSurfaceIntersection(const Geometry::Ray &localRay, QPointF &localIntersection, float &t) const;


    //inhereted from SceneGraphNode
    virtual bool draw(OpenGLData *glData);
    virtual QtwaylandSurfaceNode *getSurfaceNode(const QWaylandSurface *surface = NULL);
    virtual SceneGraphNode::RaySurfaceIntersection *intersectWithSurfaces(const Geometry::Ray &ray);






private:
    QWaylandSurface *m_surface;
    glm::mat4 m_surfaceTransform;

protected:
    GLuint composeSurface(QWaylandSurface *surface, OpenGLData *glData);
    void paintChildren(QWaylandSurface *surface, QWaylandSurface *window, OpenGLData *glData);
    void computeSurfaceTransform(float ppcm);

};

#endif // WAYLANDSURFACENODE_H
