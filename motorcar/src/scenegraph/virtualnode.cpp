#include "virtualnode.h"
using namespace motorcar;


VirtualNode::VirtualNode(SceneGraphNode &parent, const glm::mat4 &transform)
    : SceneGraphNode(parent, transform)
{
}


void VirtualNode::animate(long deltaMillis)
{
}

void VirtualNode::traverseNode(Scene *scene, long deltaMillis)
{
    SceneGraphNode::traverseNode(scene, deltaMillis);
    this->animate(deltaMillis);
}
