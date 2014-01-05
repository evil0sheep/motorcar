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

    virtual MotorcarSurfaceNode *getSurfaceNode(const QWaylandSurface *surface = NULL);
    virtual SceneGraphNode::RaySurfaceIntersection *intersectWithSurfaces(const Geometry::Ray &ray);
    glm::mat4 surfaceTransform() const;

private:
    QWaylandSurface *m_surface;
    glm::mat4 m_surfaceTransform;

protected:
    GLuint composeSurface(QWaylandSurface *surface, OpenGLData *glData);
    void paintChildren(QWaylandSurface *surface, QWaylandSurface *window, OpenGLData *glData);
    void computeSurfaceTransform(float ppcm);

};

#endif // WAYLANDSURFACENODE_H
