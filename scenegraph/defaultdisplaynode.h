#ifndef DEFAULTDISPLAYNODE_H
#define DEFAULTDISPLAYNODE_H

#include "displaynode.h"
#include "glcameranode.h"

class DefaultDisplayNode : public DisplayNode
{
public:
    DefaultDisplayNode(QObject *parent, glm::mat4 transform, OpenGLData *glData);
    virtual bool drawSurfaceNode(QtwaylandSurfaceNode *node);
    virtual bool drawSceneGraph(float dt, SceneGraphNode *sceneGraphRoot);
    virtual Geometry::Ray worldrayAtDisplayPosition(float pixelX, float pixelY);


    OpenGLData *glInfo() const;
    void setGlInfo(OpenGLData *glInfo);



private:
    OpenGLData *m_glInfo;
    GLCameraNode *m_cameraNode;
    GLCameraNode::GLViewPort computeViewport();
};

#endif // DEFAULTDISPLAYNODE_H
