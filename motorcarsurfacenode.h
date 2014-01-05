#ifndef WAYLANDSURFACENODE_H
#define WAYLANDSURFACENODE_H

#include "scenegraphnode.h"
#include "qwaylandsurface.h"
#include <QtDebug>



class MotorcarSurfaceNode : public SceneGraphNode
{
public:
    MotorcarSurfaceNode(QObject *parent, QWaylandSurface *surface);
    virtual ~MotorcarSurfaceNode();
    QWaylandSurface *surface() const;
    void setSurface(QWaylandSurface *surface);
    virtual bool draw(OpenGLData *glData);

    virtual MotorcarSurfaceNode *getSurfaceNode(const QWaylandSurface *surface = 0);


private:
    QWaylandSurface *m_surface;

protected:
    GLuint composeSurface(QWaylandSurface *surface, OpenGLData *glData);
    void paintChildren(QWaylandSurface *surface, QWaylandSurface *window, OpenGLData *glData);
    glm::mat4 surfaceTransform(float ppcm);
};

#endif // WAYLANDSURFACENODE_H
