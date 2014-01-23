#include "drawable.h"
using namespace motorcar;

Drawable::Drawable()
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

