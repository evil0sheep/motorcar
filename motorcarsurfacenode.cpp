#include "motorcarsurfacenode.h"

MotorcarSurfaceNode::MotorcarSurfaceNode(QObject *parent, QWaylandSurface *surface):
    SceneGraphNode(parent)
    , m_textureBlitter(0)
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

//TODO create a struct to hold all global data needed for rendering and pass it through the scene graph traversal
bool MotorcarSurfaceNode::draw() const
{
//    if (m_surface->visible()){
//            GLuint texture = composeSurface(surface);
//            QRect geo(surface->pos().toPoint(),surface->size());
//            m_textureBlitter->drawTexture(texture,geo,m_window->size(),0,false,surface->isYInverted());
//        return true;
//    }else{
//        return false;
//    }
}

//GLuint MotorcarSurfaceNode::composeSurface(QWaylandSurface *surface)
//{
//    GLuint texture = 0;

//    QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();
//    functions->glBindFramebuffer(GL_FRAMEBUFFER, m_surface_fbo);

//    if (surface->type() == QWaylandSurface::Shm) {
//        texture = m_textureCache->bindTexture(QOpenGLContext::currentContext(),surface->image());
//    } else if (surface->type() == QWaylandSurface::Texture) {
//        texture = surface->texture(QOpenGLContext::currentContext());
//    }

//    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
//                                       GL_TEXTURE_2D, texture, 0);
//    //paintChildren(surface,surface);
//    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
//                                       GL_TEXTURE_2D,0, 0);

//    functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    return texture;
//}

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
