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

#include "qtwaylandsurfacenode.h"

QtwaylandSurfaceNode::QtwaylandSurfaceNode(QObject *parent, QWaylandSurface *surface, glm::mat4 transform):
    SceneGraphNode(parent, transform)
{
    this->setSurface(surface);
}

QtwaylandSurfaceNode::~QtwaylandSurfaceNode(){}

QWaylandSurface *QtwaylandSurfaceNode::surface() const
{
    return m_surface;
}

void QtwaylandSurfaceNode::setSurface(QWaylandSurface *surface)
{
    m_surface = surface;
}


bool QtwaylandSurfaceNode::draw(DisplayNode *display)
{
    if (m_surface->visible()){   
        return display->drawSurfaceNode(this);
    }else{
        return false;
    }
}

GLuint QtwaylandSurfaceNode::composeSurface(QWaylandSurface *surface, OpenGLData *glData)
{
    glData->m_textureBlitter->bind();
    GLuint texture = 0;

    QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();
    functions->glBindFramebuffer(GL_FRAMEBUFFER, glData->m_surface_fbo);

    if (surface->type() == QWaylandSurface::Shm) {
        texture = glData->m_textureCache->bindTexture(QOpenGLContext::currentContext(),surface->image());
    } else if (surface->type() == QWaylandSurface::Texture) {
        texture = surface->texture(QOpenGLContext::currentContext());
    }

    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                       GL_TEXTURE_2D, texture, 0);
    paintChildren(surface,surface, glData);
    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                       GL_TEXTURE_2D,0, 0);

    functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glData->m_textureBlitter->release();
    return texture;
}

void QtwaylandSurfaceNode::paintChildren(QWaylandSurface *surface, QWaylandSurface *window, OpenGLData *glData) {

    if (surface->subSurfaces().size() == 0)
        return;

    QLinkedListIterator<QWaylandSurface *> i(surface->subSurfaces());
    while (i.hasNext()) {
        QWaylandSurface *subSurface = i.next();
        QPointF p = subSurface->mapTo(window,QPointF(0,0));
        if (subSurface->size().isValid()) {
            GLuint texture = 0;
            if (subSurface->type() == QWaylandSurface::Texture) {
                texture = subSurface->texture(QOpenGLContext::currentContext());
            } else if (surface->type() == QWaylandSurface::Shm ) {
                texture = glData->m_textureCache->bindTexture(QOpenGLContext::currentContext(),surface->image());
            }
            QRect geo(p.toPoint(),subSurface->size());
            glData->m_textureBlitter->drawTexture(texture,geo,window->size(),0,window->isYInverted(),subSurface->isYInverted());
        }
        paintChildren(subSurface,window, glData);
    }
}



void QtwaylandSurfaceNode::computeSurfaceTransform(float ppcm)
{
    if(ppcm > 0.0f){
        float ppm = ppcm * 100.f;
        glm::mat4 surfaceRotation = glm::rotate(glm::mat4(1), 180.0f ,glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 surfaceScale = glm::scale(glm::mat4(1), glm::vec3( m_surface->size().width() / ppm,  m_surface->size().height() / ppm, 1.0f));
        glm::mat4 surfaceOffset = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
        m_surfaceTransform = surfaceRotation * surfaceScale * surfaceOffset  ;

    }
}

QtwaylandSurfaceNode *QtwaylandSurfaceNode::getSurfaceNode(const QWaylandSurface *surface)
{
    if(surface == NULL || surface == this->m_surface) {
        return this;
    }else{
        return SceneGraphNode::getSurfaceNode(surface);
    }

}

bool QtwaylandSurfaceNode::computeLocalSurfaceIntersection(const Geometry::Ray &localRay, QPointF &localIntersection, float &t) const
{
    Geometry::Plane surfacePlane = Geometry::Plane(glm::vec3(0.0f), glm::vec3(0.0f,0.0f,1.0f));
    if(glm::dot(localRay.d, surfacePlane.n) == 0) return false;

    Geometry::Ray transformedRay = localRay.transform(glm::inverse(surfaceTransform()));

    t = surfacePlane.intersect(transformedRay);
    glm::vec3 intersection = transformedRay.solve(t);

    glm::vec3 coords= intersection * glm::vec3(m_surface->size().width(), m_surface->size().height(), 0);
    localIntersection =  QPointF(coords.x, coords.y);

    return true;
}

SceneGraphNode::RaySurfaceIntersection *QtwaylandSurfaceNode::intersectWithSurfaces(const Geometry::Ray &ray)
{


    SceneGraphNode::RaySurfaceIntersection *closestSubtreeIntersection = SceneGraphNode::intersectWithSurfaces(ray);
    Geometry::Ray localRay = ray.transform(glm::inverse(transform()));

    float t;
    QPointF localIntersection;
    bool isIntersected = computeLocalSurfaceIntersection(localRay, localIntersection, t);

    //qDebug() << "intersection: " << localIntersection ;

    if(isIntersected && (closestSubtreeIntersection == NULL || t < closestSubtreeIntersection-> t)){

        if(localIntersection.x() >= 0 && localIntersection.x() <= m_surface->size().width() && localIntersection.y() >= 0 && localIntersection.y() <= m_surface->size().height()){
            return new SceneGraphNode::RaySurfaceIntersection(this, localIntersection, ray, t);
        }else{
            return NULL;
        }

    }else{
        return closestSubtreeIntersection;
    }



}

glm::mat4 QtwaylandSurfaceNode::surfaceTransform() const
{
    return m_surfaceTransform;
}





