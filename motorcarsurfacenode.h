#ifndef WAYLANDSURFACENODE_H
#define WAYLANDSURFACENODE_H

#include "scenegraphnode.h"
#include "qwaylandsurface.h"
#include <QtDebug>
#include "glm/gtc/matrix_inverse.hpp"


class MotorcarSurfaceNode : public SceneGraphNode
{
public:
    MotorcarSurfaceNode(QObject *parent, QWaylandSurface *surface, glm::mat4 transform = glm::mat4(1));
    virtual ~MotorcarSurfaceNode();
    QWaylandSurface *surface() const;
    void setSurface(QWaylandSurface *surface);
    glm::mat4 surfaceTransform() const;



    //takes a ray in the local Node space and returns whether or not the ray insersects the plane of this surface;
    // t: the ray's intersection distance to the surface
    // localIntersection : the ray's intersection with the surface in wayland "surface local coordinates" as a QPoint for use with QTWayland
    bool computeLocalSurfaceIntersection(const Geometry::Ray &localRay, QPointF &localIntersection, float &t) const;


    //inhereted from SceneGraphNode
    virtual bool draw(OpenGLData *glData);
    virtual MotorcarSurfaceNode *getSurfaceNode(const QWaylandSurface *surface = NULL);
    virtual SceneGraphNode::RaySurfaceIntersection *intersectWithSurfaces(const Geometry::Ray &ray);






private:
    QWaylandSurface *m_surface;
    glm::mat4 m_surfaceTransform;

protected:
    GLuint composeSurface(QWaylandSurface *surface, OpenGLData *glData);
    void paintChildren(QWaylandSurface *surface, QWaylandSurface *window, OpenGLData *glData);
    void computeSurfaceTransform(float ppcm);

};

#endif // WAYLANDSURFACENODE_H
