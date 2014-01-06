#include "motorcarsurfacenode.h"

MotorcarSurfaceNode::MotorcarSurfaceNode(QObject *parent, QWaylandSurface *surface):
    SceneGraphNode(parent)
{
    this->setSurface(surface);
}

MotorcarSurfaceNode::~MotorcarSurfaceNode(){}

QWaylandSurface *MotorcarSurfaceNode::surface() const
{
    return m_surface;
}

void MotorcarSurfaceNode::setSurface(QWaylandSurface *surface)
{
    m_surface = surface;
}


bool MotorcarSurfaceNode::draw(OpenGLData *glData)
{
    if (m_surface->visible()){
            GLuint texture = composeSurface(m_surface, glData);
            //QRect geo(m_surface->pos().toPoint(),m_surface->size());
            //glData->m_textureBlitter->drawTexture(texture,geo,glData->m_window->size(),0,false,m_surface->isYInverted());
            glData->m_surfaceShader->bind();
            glViewport(0,0,glData->m_window->size().width(),glData->m_window->size().height());
            GLint aPositionLocation =  glData->m_surfaceShader->attributeLocation("aPosition");
            GLint aTexCoordLocation =  glData->m_surfaceShader->attributeLocation("aTexCoord");
            GLint uMVPMatLocation = glData->m_surfaceShader->uniformLocation("uMVPMatrix");

            if(aPositionLocation < 0 || aTexCoordLocation < 0 || uMVPMatLocation < 0){
                qDebug() << "problem with surface shader handles: " << aPositionLocation << ", " << aTexCoordLocation << ", " << uMVPMatLocation ;
            }

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

            computeSurfaceTransform(glData->ppcm());
            QMatrix4x4 MVPMatrix(glm::value_ptr(glData->viewProjectionMatrix() * this->worldTransform() * this->surfaceTransform()));

            QOpenGLContext *currentContext = QOpenGLContext::currentContext();
            currentContext->functions()->glEnableVertexAttribArray(aPositionLocation);
            currentContext->functions()->glEnableVertexAttribArray(aTexCoordLocation);

            currentContext->functions()->glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 0, vertexCoordinates);
            currentContext->functions()->glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, textureCoordinates);
            glData->m_surfaceShader->setUniformValue(uMVPMatLocation, MVPMatrix);

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

GLuint MotorcarSurfaceNode::composeSurface(QWaylandSurface *surface, OpenGLData *glData)
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

void MotorcarSurfaceNode::paintChildren(QWaylandSurface *surface, QWaylandSurface *window, OpenGLData *glData) {

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



void MotorcarSurfaceNode::computeSurfaceTransform(float ppcm)
{
    if(ppcm > 0){
        float ppm = ppcm * 100.f;
        m_surfaceTransform = glm::scale(glm::mat4(), glm::vec3(m_surface->size().width() / ppm, -1 * m_surface->size().height() / ppm, 1));
    }



}

MotorcarSurfaceNode *MotorcarSurfaceNode::getSurfaceNode(const QWaylandSurface *surface)
{
    if(surface == NULL || surface == this->m_surface) {
        return this;
    }else{
        return SceneGraphNode::getSurfaceNode(surface);
    }

}

SceneGraphNode::RaySurfaceIntersection *MotorcarSurfaceNode::intersectWithSurfaces(const Geometry::Ray &ray)
{
    SceneGraphNode::RaySurfaceIntersection *closestSubtreeIntersection = SceneGraphNode::intersectWithSurfaces(ray);


    Geometry::Ray transformedRay = ray.transform(m_inverseTransform).transform( glm::inverse(surfaceTransform()) );

    Geometry::Plane surfacePlane = Geometry::Plane(glm::vec3(0), glm::vec3(0,0,1));
    float t = surfacePlane.intersect(transformedRay);
    if(closestSubtreeIntersection == NULL || t < closestSubtreeIntersection->t){
        glm::vec3 pos = transformedRay.solve(t) * glm::vec3(-1, 1, 1);
        if(pos.x >= 0 && pos.x <=1 && pos.y >= 0 && pos.y <= 1){
            return new SceneGraphNode::RaySurfaceIntersection(this, glm::vec2(pos.x, pos.y) * glm::vec2(m_surface->size().width(), m_surface->size().height()), ray, t);
        }else{
            return NULL;
        }

    }else{
        return closestSubtreeIntersection;
    }



}

glm::mat4 MotorcarSurfaceNode::surfaceTransform() const
{
    return m_surfaceTransform;
}
