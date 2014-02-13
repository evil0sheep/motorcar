#include "physicalnode.h"
using namespace motorcar;


PhysicalNode::PhysicalNode(PhysicalNode *parent, const glm::mat4 &transform)
    :SceneGraphNode(parent, transform)
{
}

PhysicalNode::PhysicalNode()
    :SceneGraphNode()
{
}
