#include "drawable.h"
#include "../scene.h"
using namespace motorcar;

Drawable::Drawable(SceneGraphNode &parent, const glm::mat4 &transform)
    : VirtualNode(parent, transform)
{
}




void Drawable::traverseNode(Scene *scene, long deltaMillis)
{
    VirtualNode::traverseNode(scene, deltaMillis);
    this->draw(scene);
}

void Drawable::draw(Scene *scene)
{
    for(Display *display : scene->displays()){
        display->renderDrawable(this);
    }
}



