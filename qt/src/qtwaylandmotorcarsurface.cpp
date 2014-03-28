#include "qtwaylandmotorcarsurface.h"
using namespace qtmotorcar;

QtWaylandMotorcarSurface::QtWaylandMotorcarSurface(QWaylandSurface *surface, QtWaylandMotorcarCompositor *compositor, SurfaceType type)
    :motorcar::WaylandSurface(type)
    , m_surface(surface)
    , m_compositor(compositor)
    , m_ownsTexture(false)
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

glm::ivec2 QtWaylandMotorcarSurface::position()
{
    return glm::ivec2(m_surface->pos().x(), m_surface->pos().y());
}

motorcar::WaylandSurface *QtWaylandMotorcarSurface::parentSurface()
{
    if(m_surface->parentSurface() != NULL){
        return m_compositor->getSurfaceNode(m_surface->parentSurface())->surface();
    }else{
        return NULL;
    }

}

void QtWaylandMotorcarSurface::prepare()
{
    if (m_ownsTexture){
        glDeleteTextures(1, &m_textureID);
    }
    m_textureID = composeSurface(m_surface, &m_ownsTexture, m_compositor->glData());
}

void QtWaylandMotorcarSurface::sendEvent(const motorcar::Event &event)
{

    //std::cout << "recieved mouse event in qt wayland surface" << std::endl;
    QWaylandInputDevice *input = m_compositor->defaultInputDevice();

    int eventType = static_cast<int>(event.type());

    if(eventType == motorcar::Event::EventType::MOUSE){
        motorcar::MouseEvent mouseEvent = dynamic_cast<const motorcar::MouseEvent &>(event);

        QPointF localPos(mouseEvent.localPosition().x, mouseEvent.localPosition().y);

        event.seat()->setPointerFocus(this, mouseEvent.localPosition());
        event.seat()->setKeyboardFocus(this);

        Qt::MouseButton button;
        switch(mouseEvent.button()){
        case motorcar::MouseEvent::Button::LEFT:
            //std::cout << "left mouse button" << std::endl;
            button = Qt::LeftButton;
            break;
        case motorcar::MouseEvent::Button::RIGHT:
            //std::cout << "right mouse button" << std::endl;
            button = Qt::RightButton;
            break;
        case motorcar::MouseEvent::Button::MIDDLE:
            //std::cout << "middle mouse button" << std::endl;
            button = Qt::MiddleButton;
            break;
        default:
            //std::cout << "no mouse button" << std::endl;
            button = Qt::NoButton;
            break;
        }

        switch(mouseEvent.event()){
        case motorcar::MouseEvent::Event::BUTTON_PRESS:
            //std::cout << "button press event" << std::endl;
            input->sendMousePressEvent(button, localPos);
            break;
        case motorcar::MouseEvent::Event::BUTTON_RELEASE:
            //std::cout << "button release event" << std::endl;
            input->sendMouseReleaseEvent(button, localPos);
            break;
        case motorcar::MouseEvent::Event::MOVE:
            //std::cout << "mouse move event" << std::endl;
            input->sendMouseMoveEvent(m_surface, localPos);
            break;
        default:
            break;
        }
    } else if(eventType == motorcar::Event::EventType::KEYBOARD){
        motorcar::KeyboardEvent keyboardEvent = dynamic_cast<const motorcar::KeyboardEvent &>(event);
        std::cout << "delivering keyboard event to surface " << this << std::endl;
        event.seat()->setKeyboardFocus(this);
        switch(keyboardEvent.event()){
        case motorcar::KeyboardEvent::Event::KEY_PRESS:
             input->sendKeyPressEvent(keyboardEvent.keyCode());
             break;
        case motorcar::KeyboardEvent::Event::KEY_RELEASE:
             input->sendKeyReleaseEvent(keyboardEvent.keyCode());
             break;
        default:
            break;
        }
    }

}

bool QtWaylandMotorcarSurface::valid()
{
    return m_surface != NULL;
}


//GLuint QtWaylandMotorcarSurface::composeSurface(QWaylandSurface *surface, OpenGLData *glData)
//{
//    glData->m_textureBlitter->bind();
//    GLuint texture = 0;

//    QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();
//    functions->glBindFramebuffer(GL_FRAMEBUFFER, glData->m_surface_fbo);

//    if (surface->type() == QWaylandSurface::Shm) {
//        texture = glData->m_textureCache->bindTexture(surface->image());
//    } else if (surface->type() == QWaylandSurface::Texture) {
//        texture = surface->texture();
//    }

//    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
//                                       GL_TEXTURE_2D, texture, 0);
//    paintChildren(surface,surface, glData);
//    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
//                                       GL_TEXTURE_2D,0, 0);

//    functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    glData->m_textureBlitter->release();
//    return texture;
//}

//void QtWaylandMotorcarSurface::paintChildren(QWaylandSurface *surface, QWaylandSurface *window, OpenGLData *glData) {

//    if (surface->subSurfaces().size() == 0)
//        return;

//    QLinkedListIterator<QWaylandSurface *> i(surface->subSurfaces());
//    while (i.hasNext()) {
//        QWaylandSurface *subSurface = i.next();
//        QPointF p = subSurface->mapTo(window,QPointF(0,0));
//        if (subSurface->size().isValid()) {
//            GLuint texture = 0;
//            if (subSurface->type() == QWaylandSurface::Texture) {
//                texture = subSurface->texture();
//            } else if (surface->type() == QWaylandSurface::Shm ) {
//                texture = glData->m_textureCache->bindTexture(QOpenGLContext::currentContext(),surface->image());
//            }
//            QRect geo(p.toPoint(),subSurface->size());
//            glData->m_textureBlitter->drawTexture(texture,geo,window->size(),0,window->isYInverted(),subSurface->isYInverted());
//        }
//        paintChildren(subSurface,window, glData);
//    }
//}




static GLuint textureFromImage(const QImage &image)
{
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    QImage tx = image.convertToFormat(QImage::Format_RGBA8888_Premultiplied);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tx.width(), tx.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tx.constBits());
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

GLuint QtWaylandMotorcarSurface::composeSurface(QWaylandSurface *surface, bool *textureOwned, OpenGLData *glData)
{
    GLuint texture = 0;

    QSize windowSize = surface->size();
    surface->advanceBufferQueue();

    QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();
    functions->glBindFramebuffer(GL_FRAMEBUFFER, glData->m_surface_fbo);

    if (surface->type() == QWaylandSurface::Shm) {
        texture = textureFromImage(surface->image());
        *textureOwned = true;
    } else if (surface->type() == QWaylandSurface::Texture) {
        texture = surface->texture();
        *textureOwned = false;
    }

    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                       GL_TEXTURE_2D, texture, 0);
    paintChildren(surface, surface,windowSize, glData);
    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                       GL_TEXTURE_2D,0, 0);

    functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);


    return texture;
}

void QtWaylandMotorcarSurface::paintChildren(QWaylandSurface *surface, QWaylandSurface *window, const QSize &windowSize, OpenGLData *glData) {

    if (surface->subSurfaces().size() == 0)
        return;

    QLinkedListIterator<QWaylandSurface *> i(surface->subSurfaces());
    while (i.hasNext()) {
        QWaylandSurface *subSurface = i.next();
        QPointF p = subSurface->mapTo(window,QPointF(0,0));
        QSize subSize = subSurface->size();
        subSurface->advanceBufferQueue();
        if (subSize.isValid()) {
            GLuint texture = 0;
            if (subSurface->type() == QWaylandSurface::Texture) {
                texture = subSurface->texture();
            } else if (surface->type() == QWaylandSurface::Shm) {
                texture = textureFromImage(subSurface->image());
            }
            QRect geo(p.toPoint(),subSize);
            if (texture > 0)
                glData->m_textureBlitter->drawTexture(texture,geo,windowSize,0,window->isYInverted(),subSurface->isYInverted());
            if (surface->type() == QWaylandSurface::Shm)
                glDeleteTextures(1, &texture);
        }
        paintChildren(subSurface,window,windowSize, glData);
    }
}




QWaylandSurface *QtWaylandMotorcarSurface::surface() const
{
    return m_surface;
}

void QtWaylandMotorcarSurface::setSurface(QWaylandSurface *surface)
{
    m_surface = surface;
}
