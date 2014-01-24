#include "drawable.h"
using namespace motorcar;

Drawable::Drawable(SceneGraphNode &parent, const glm::mat4 &transform)
    : VirtualNode(parent, transform)
{
}




void Drawable::traverseNode(long deltaMillis)
{
    VirtualNode::traverseNode(deltaMillis);
    this->draw();
}

void Drawable::draw()
{
}



