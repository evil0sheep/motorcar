#include "qtwaylandmotorcaropenglcontext.h"



qtmotorcar::QtWaylandMotorcarOpenGLContext::QtWaylandMotorcarOpenGLContext(QOpenGLWindow *window)
    :m_window(window)
{
}

glm::ivec2 qtmotorcar::QtWaylandMotorcarOpenGLContext::defaultFramebufferSize()
{
   return glm::ivec2(m_window->size().width(), m_window->size().height());
}

void qtmotorcar::QtWaylandMotorcarOpenGLContext::makeCurrent()
{
    m_window->makeCurrent();
}
