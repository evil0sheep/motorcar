#include "virtualnode.h"
#include "scene.h"
using namespace motorcar;


VirtualNode::VirtualNode(SceneGraphNode *parent, const glm::mat4 &transform)
    : SceneGraphNode(parent, transform)
{
}


void VirtualNode::animate(long deltaMillis)
{
}

void VirtualNode::handleFrameBegin(Scene *scene)
{
    animate(scene->latestTimestampChange());
}



void VirtualNode::setParentNode(SceneGraphNode *parent)
{
    SceneGraphNode::setParentNode(parent);
}
