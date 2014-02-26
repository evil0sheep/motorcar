#include "qtwaylandmotorcarsurface.h"
using namespace qtmotorcar;

QtWaylandMotorcarSurface::QtWaylandMotorcarSurface(QWaylandSurface *surface, QtWaylandMotorcarCompositor *compositor)
    : m_surface(surface)
    , m_compositor(compositor)
{

}

GLuint QtWaylandMotorcarSurface::texture()
{

    return m_textureID;
}

glm::ivec2 QtWaylandMotorcarSurface::size()
{
    return glm::ivec2(m_surface->size().width(), m_surface->size().height());
}

void QtWaylandMotorcarSurface::prepare()
{
    m_textureID = composeSurface(m_surface, m_compositor->glData());
}

void QtWaylandMotorcarSurface::sendMouseEvent(motorcar::WaylandSurface::MouseEvent eventType, motorcar::WaylandSurface::MouseButton buttonId, glm::vec2 localPostion)
{
    //std::cout << "recieved mouse event in qt wayland surface" << std::endl;
    QWaylandInputDevice *input = m_compositor->defaultInputDevice();


    QPointF localPos(localPostion.x, localPostion.y);

    if(input->mouseFocus() != m_surface){
        input->setMouseFocus(m_surface, localPos);
    }

    Qt::MouseButton button;
    switch(buttonId){
    case motorcar::WaylandSurface::MouseButton::LEFT:
        std::cout << "left mouse button" << std::endl;
        button = Qt::LeftButton;
        break;
    case motorcar::WaylandSurface::MouseButton::RIGHT:
        std::cout << "right mouse button" << std::endl;
        button = Qt::RightButton;
        break;
    case motorcar::WaylandSurface::MouseButton::MIDDLE:
        std::cout << "middle mouse button" << std::endl;
        button = Qt::MiddleButton;
        break;
    default:
        //std::cout << "no mouse button" << std::endl;
        button = Qt::NoButton;
        break;
    }

    switch(eventType){
    case motorcar::WaylandSurface::MouseEvent::BUTTON_PRESS:
        std::cout << "button press event" << std::endl;
        if (m_surface && input->keyboardFocus() != m_surface) {
            input->setKeyboardFocus(m_surface);
        }
        input->sendMousePressEvent(button, localPos);
        break;
    case motorcar::WaylandSurface::MouseEvent::BUTTON_RELEASE:
        std::cout << "button release event" << std::endl;
        input->sendMouseReleaseEvent(button, localPos);
        break;
    case motorcar::WaylandSurface::MouseEvent::MOVE:
        //std::cout << "mouse move event" << std::endl;
        input->sendMouseMoveEvent(m_surface, localPos);
        break;
    default:
        break;
    }
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




