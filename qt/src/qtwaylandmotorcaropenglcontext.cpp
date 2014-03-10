#include "qtwaylandmotorcaropenglcontext.h"

using namespace qtmotorcar;

QtWaylandMotorcarOpenGLContext::QtWaylandMotorcarOpenGLContext(QOpenGLWindow *window)
    :m_window(window)
{
}

glm::ivec2 QtWaylandMotorcarOpenGLContext::defaultFramebufferSize()
{
   return glm::ivec2(m_window->size().width(), m_window->size().height());
}

void QtWaylandMotorcarOpenGLContext::makeCurrent()
{
    m_window->makeCurrent();
}
QOpenGLWindow *QtWaylandMotorcarOpenGLContext::window() const
{
    return m_window;
}

void QtWaylandMotorcarOpenGLContext::setWindow(QOpenGLWindow *window)
{
    m_window = window;
}

