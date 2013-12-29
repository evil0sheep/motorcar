#include "motorcarsurfacenode.h"

MotorcarSurfaceNode::MotorcarSurfaceNode(QObject *parent, QWaylandSurface *surface):
    SceneGraphNode(parent)
{
    this->setSurface(surface);
}

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
            QRect geo(m_surface->pos().toPoint(),m_surface->size());
            glData->m_textureBlitter->drawTexture(texture,geo,glData->m_window->size(),0,false,m_surface->isYInverted());
        return true;
    }else{
        return false;
    }
}

GLuint MotorcarSurfaceNode::composeSurface(QWaylandSurface *surface, OpenGLData *glData)
{
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

MotorcarSurfaceNode *MotorcarSurfaceNode::getSurfaceNode(const QWaylandSurface *surface)
{
    if(surface == NULL || surface == this->m_surface) return this;
    foreach (SceneGraphNode *child, m_childNodes) {
        if (child != NULL){
            MotorcarSurfaceNode *node = child->getSurfaceNode(surface);
            if(node) return node;
        }
    }
    return NULL;
}
