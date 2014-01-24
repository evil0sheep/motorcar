#ifndef QTWAYLANDMOTORCARSURFACE_H
#define QTWAYLANDMOTORCARSURFACE_H

#include "../../motorcar/src/motorcar.h"
#include "opengldata.h"
#include "qwaylandsurface.h"
#include "qtwaylandmotorcarcompositor.h"
#include <QtDebug>
#include <glm/glm.hpp>

class QtWaylandMotorcarSurface : public motorcar::WaylandSurface
{
public:
    QtWaylandMotorcarSurface(QWaylandSurface *surface, QtWaylandMotorcarCompositor *compositor);
    ~QtWaylandMotorcarSurface();

    GLuint getTexture();
    glm::ivec2 getSize();

private:
    GLuint m_textureID;
    QWaylandSurface *m_surface;
    QtWaylandMotorcarCompositor *m_compositor;

    GLuint composeSurface(QWaylandSurface *surface, OpenGLData *glData);
    void paintChildren(QWaylandSurface *surface, QWaylandSurface *window, OpenGLData *glData);
    void computeSurfaceTransform(float ppcm);



};

#endif // QTWAYLANDMOTORCARSURFACE_H
