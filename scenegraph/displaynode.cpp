#include "displaynode.h"



DisplayNode::DisplayNode(QObject *parent, glm::mat4 transform)
    :SceneGraphNode(parent, transform)
{
}

DisplayNode::~DisplayNode()
{
}


bool DisplayNode::drawSceneGraph(float dt, SceneGraphNode *sceneGraphRoot)
{
    return sceneGraphRoot->traverse(dt, this);
}

