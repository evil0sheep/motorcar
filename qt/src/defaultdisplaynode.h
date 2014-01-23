#ifndef DEFAULTDISPLAYNODE_H
#define DEFAULTDISPLAYNODE_H

#include "../../motorcar/src/motorcar.h"

using namespace motorcar;
namespace qtmotorcar {
class DefaultDisplayNode : public motorcar::Display
{
public:
    DefaultDisplayNode(SceneGraphNode &parent, glm::mat4 transform, OpenGLData *glData);
    virtual bool drawSurfaceNode(QtwaylandSurfaceNode *node);
    virtual bool drawSceneGraph(float dt, SceneGraphNode *sceneGraphRoot);
    virtual Geometry::Ray worldRayAtDisplayPosition(float pixelX, float pixelY);


    OpenGLData *glInfo() const;
    void setGlInfo(OpenGLData *glInfo);



private:
    OpenGLData *m_glInfo;
    GLCameraNode *m_cameraNode;

};
}


#endif // DEFAULTDISPLAYNODE_H
