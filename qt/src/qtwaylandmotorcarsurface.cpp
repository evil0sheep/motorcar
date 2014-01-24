#include "qtwaylandmotorcarsurface.h"

QtWaylandMotorcarSurface::QtWaylandMotorcarSurface(QWaylandSurface *surface, QtWaylandMotorcarCompositor *compositor)
    : m_surface(surface)
    , m_compositor(compositor)
{

}

GLuint QtWaylandMotorcarSurface::getTexture()
{
    m_textureID = composeSurface(m_surface, m_compositor->glData());
    return m_textureID;
}

glm::ivec2 QtWaylandMotorcarSurface::getSize()
{
    return glm::ivec2(m_surface->size().width(), m_surface->size().height());
}

GLuint QtWaylandMotorcarSurface::composeSurface(QWaylandSurface *surface, OpenGLData *glData)
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

void QtWaylandMotorcarSurface::paintChildren(QWaylandSurface *surface, QWaylandSurface *window, OpenGLData *glData) {

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



