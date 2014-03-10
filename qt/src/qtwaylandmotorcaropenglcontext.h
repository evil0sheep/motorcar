#ifndef QTWAYLANDMOTORCAROPENGLCONTEXT_H
#define QTWAYLANDMOTORCAROPENGLCONTEXT_H
#include "../../motorcar/src/motorcar.h"
#include "qopenglwindow.h"

namespace qtmotorcar{
class QtWaylandMotorcarOpenGLContext : public motorcar::OpenGLContext
{
public:
    QtWaylandMotorcarOpenGLContext(QOpenGLWindow *window);
    glm::ivec2 defaultFramebufferSize() override;
    void makeCurrent() override;

    QOpenGLWindow *window() const;
    void setWindow(QOpenGLWindow *window);

private:
    QOpenGLWindow *m_window;
};
}


#endif // QTWAYLANDMOTORCAROPENGLCONTEXT_H
