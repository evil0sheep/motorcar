#ifndef WAYLANDSURFACENODE_H
#define WAYLANDSURFACENODE_H

#include "scenegraphnode.h"
#include "qwaylandsurface.h"



class MotorcarSurfaceNode : public SceneGraphNode
{
public:
    MotorcarSurfaceNode(QObject *parent, QWaylandSurface *surface);
    QWaylandSurface *surface() const;
    void setSurface(QWaylandSurface *surface);
    virtual bool draw(OpenGLData *glData);

    virtual MotorcarSurfaceNode *getSurfaceNode(const QWaylandSurface *surface = 0);


private:
    QWaylandSurface *m_surface;

protected:
    GLuint composeSurface(QWaylandSurface *surface, OpenGLData *glData);
    void paintChildren(QWaylandSurface *surface, QWaylandSurface *window, OpenGLData *glData);
};

#endif // WAYLANDSURFACENODE_H
