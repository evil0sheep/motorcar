#include "abstractnode.h"

AbstractNode::AbstractNode()
{
}

AbstractNode::~AbstractNode()
{
}

void AbstractNode::traverseNode(long deltaMillis)
{
}

void AbstractNode::traverseSceneGraph(long deltaMillis)
{
}

glm::mat4 AbstractNode::transform() const
{
}

void AbstractNode::setTransform(const glm::mat4 &value)
{
}

glm::mat4 AbstractNode::inverseTransform() const
{
}

glm::mat4 AbstractNode::worldTransform() const
{
}
