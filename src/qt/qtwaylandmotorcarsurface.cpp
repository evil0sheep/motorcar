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

#include <qt/qtwaylandmotorcarsurface.h>

using namespace qtmotorcar;

QtWaylandMotorcarSurface::QtWaylandMotorcarSurface(QWaylandSurface *surface, QtWaylandMotorcarCompositor *compositor, SurfaceType type)
    :motorcar::WaylandSurface(type)
    , m_surface(surface)
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

void QtWaylandMotorcarSurface::setSize(glm::ivec2 newSize)
{
    this->surface()->requestSize(QSize(newSize.x, newSize.y));
}

glm::ivec2 QtWaylandMotorcarSurface::position()
{
    QWaylandSurfaceView *l_view = m_surface->views().first();
    return glm::ivec2(l_view->pos().x(), l_view->pos().y());
}

motorcar::WaylandSurface *QtWaylandMotorcarSurface::parentSurface()
{
    if(m_surface->parentSurface() != NULL){
        return m_compositor->getMotorcarSurface(m_surface->parentSurface());
    }else{
        return NULL;
    }
}

void QtWaylandMotorcarSurface::prepare()
{
    m_textureID = composeSurface(m_surface, m_compositor->glData());
}

void QtWaylandMotorcarSurface::sendEvent(const motorcar::Event &event)
{
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
                button = Qt::LeftButton;
            break;
            case motorcar::MouseEvent::Button::RIGHT:
                button = Qt::RightButton;
            break;
            case motorcar::MouseEvent::Button::MIDDLE:
                button = Qt::MiddleButton;
            break;
            default:
                button = Qt::NoButton;
            break;
        }
        switch(mouseEvent.event()){
            case motorcar::MouseEvent::Event::BUTTON_PRESS:
                input->sendMousePressEvent(button, localPos);
            break;
            case motorcar::MouseEvent::Event::BUTTON_RELEASE:
                input->sendMouseReleaseEvent(button, localPos);
            break;
            case motorcar::MouseEvent::Event::MOVE:
                input->sendMouseMoveEvent(m_surface->views().first(), localPos);
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

GLuint QtWaylandMotorcarSurface::composeSurface(QWaylandSurface *surface, OpenGLData *glData)
{
    GLuint texture = 0;
    QSize windowSize = surface->size();
    QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();
    functions->glBindFramebuffer(GL_FRAMEBUFFER, glData->m_surface_fbo);
    if (surface->type() == QWaylandSurface::Shm) {
        texture = textureFromImage((dynamic_cast<BufferAttacher *>(surface->bufferAttacher()))->image());
    } else if (surface->type() == QWaylandSurface::Texture) {
        texture = (static_cast<BufferAttacher *>(surface->bufferAttacher()))->texture;
    }
    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                      GL_TEXTURE_2D, texture, 0);
    paintChildren(surface, surface,windowSize, glData);
    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                      GL_TEXTURE_2D,0, 0);
    functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return texture;
}

void QtWaylandMotorcarSurface::paintChildren(QWaylandSurface *surface, QWaylandSurface *window, const QSize &windowSize, OpenGLData *glData)
{
    if (surface->subSurfaces().size() == 0)
        return;

    QLinkedListIterator<QWaylandSurface *> i(surface->subSurfaces());
    while (i.hasNext()) {
        QWaylandSurface *subSurface = i.next();
        QPointF p = subSurface->views().first()->pos() + surface->views().first()->pos();
        QSize subSize = subSurface->size();
        if (subSize.isValid()) {
            GLuint texture = 0;
            if (subSurface->type() == QWaylandSurface::Texture) {
                texture = (dynamic_cast<BufferAttacher*>(subSurface->views().first()->surface()->bufferAttacher()))->texture;
            } else if (surface->type() == QWaylandSurface::Shm) {
                texture = textureFromImage((dynamic_cast<BufferAttacher*>(subSurface->views().first()->surface()->bufferAttacher()))->image());
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
