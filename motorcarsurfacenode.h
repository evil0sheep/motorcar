#ifndef WAYLANDSURFACENODE_H
#define WAYLANDSURFACENODE_H

#include "scenegraphnode.h"
#include "qwaylandsurface.h"
#include "textureblitter.h"


class MotorcarSurfaceNode : public SceneGraphNode
{
public:
    MotorcarSurfaceNode(QObject *parent, QWaylandSurface *surface);
    QWaylandSurface *surface() const;
    void setSurface(QWaylandSurface *surface);
    virtual bool draw() const;

    virtual MotorcarSurfaceNode *getSurfaceNode(const QWaylandSurface *surface = 0);


private:
    QWaylandSurface *m_surface;
    TextureBlitter *m_textureBlitter;
    GLuint composeSurface(QWaylandSurface *surface);
};

#endif // WAYLANDSURFACENODE_H
