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


bool QtwaylandSurfaceNode::draw(OpenGLData *glData)
{
    if (m_surface->visible()){
            GLuint texture = composeSurface(m_surface, glData);
            //QRect geo(m_surface->pos().toPoint(),m_surface->size());
            //glData->m_textureBlitter->drawTexture(texture,geo,glData->m_window->size(),0,false,m_surface->isYInverted());


            const GLfloat textureCoordinates[] = {
                0, 0,
                0, 1,
                1, 1,
                1, 0
            };
            const GLfloat vertexCoordinates[] ={
                0, 0, 0,
                0, 1, 0,
                1, 1, 0,
                1, 0, 0
            };

            glData->m_surfaceShader->bind();
            glViewport(0,0,glData->m_window->size().width(),glData->m_window->size().height());

            GLint aPositionLocation =  glData->m_surfaceShader->attributeLocation("aPosition");
            GLint aTexCoordLocation =  glData->m_surfaceShader->attributeLocation("aTexCoord");
            GLint uMVPMatLocation = glData->m_surfaceShader->uniformLocation("uMVPMatrix");
//            GLint uModelMatrix = glData->m_surfaceShader->uniformLocation("uModelMatrix");
//            GLint uViewMatrix = glData->m_surfaceShader->uniformLocation("uViewMatrix");
//            GLint uProjectionMatrix = glData->m_surfaceShader->uniformLocation("uProjectionMatrix");

            if(aPositionLocation < 0 || aTexCoordLocation < 0 || uMVPMatLocation< 0 ){//|| uModelMatrix < 0 || uViewMatrix < 0 || uProjectionMatrix < 0){
                qDebug() << "problem with surface shader handles: " << aPositionLocation << ", "<< aTexCoordLocation << ", " << uMVPMatLocation ;// << ", " << uModelMatrix << ", "  << uViewMatrix << ", "  << uProjectionMatrix ;
            }

            computeSurfaceTransform(glData->ppcm());




            QMatrix4x4 MVPMatrix(glm::value_ptr(glm::transpose( glData->projectionMatrix() * glData->viewMatrix() *  this->worldTransform() * this->surfaceTransform())));
//            QMatrix4x4 modelMatrix(glm::value_ptr(this->worldTransform()));
//            QMatrix4x4 viewMatrix(glm::value_ptr(glData->viewMatrix()));
//            QMatrix4x4 projectionMatrix(glm::value_ptr(glData->projectionMatrix()));

            QOpenGLContext *currentContext = QOpenGLContext::currentContext();
            currentContext->functions()->glEnableVertexAttribArray(aPositionLocation);
            currentContext->functions()->glEnableVertexAttribArray(aTexCoordLocation);

            currentContext->functions()->glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 0, vertexCoordinates);
            currentContext->functions()->glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, textureCoordinates);
            glData->m_surfaceShader->setUniformValue(uMVPMatLocation, MVPMatrix);
//            glData->m_surfaceShader->setUniformValue(uModelMatrix, modelMatrix);
//            glData->m_surfaceShader->setUniformValue(uViewMatrix, viewMatrix);
//            glData->m_surfaceShader->setUniformValue(uProjectionMatrix, projectionMatrix);

            //Geometry::printMatrix(glData->projectionMatrix() * (glData->viewMatrix() *  this->worldTransform() * this->surfaceTransform()));
//            Geometry::printMatrix( this->surfaceTransform());


            glBindTexture(GL_TEXTURE_2D, texture);

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

            glBindTexture(GL_TEXTURE_2D, 0);

            currentContext->functions()->glDisableVertexAttribArray(aPositionLocation);
            currentContext->functions()->glDisableVertexAttribArray(aTexCoordLocation);


            glData->m_surfaceShader->release();
        return true;
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
    if(ppcm > 0){
        float ppm = ppcm * 100.f;
        glm::mat4 surfaceRotation = glm::rotate(glm::mat4(1), 180.f ,glm::vec3(0, 0, 1));
        glm::mat4 surfaceScale = glm::scale(glm::mat4(1), glm::vec3( m_surface->size().width() / ppm,  m_surface->size().height() / ppm, 1));
        glm::mat4 surfaceOffset = glm::translate(glm::mat4(1), glm::vec3(-0.5f, -0.5f, 0));
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
    Geometry::Plane surfacePlane = Geometry::Plane(glm::vec3(0), glm::vec3(0,0,1));
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





