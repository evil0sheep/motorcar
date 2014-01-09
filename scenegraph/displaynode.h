#ifndef DISPLAYNODE_H
#define DISPLAYNODE_H
#include "qtwaylandsurfacenode.h"

class DisplayNode : public SceneGraphNode
{
public:
    DisplayNode(QObject *parent = 0, glm::mat4 transform = glm::mat4(1));
    virtual ~DisplayNode();

    virtual bool drawSurfaceNode(QtwaylandSurfaceNode *node) = 0;
    virtual bool drawSceneGraph(float dt, SceneGraphNode *sceneGraphRoot);

    virtual Geometry::Ray worldRayAtDisplayPosition(float pixelX, float pixelY) = 0;



};

#endif // DISPLAYNODE_H
