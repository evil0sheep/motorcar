#include "drawable.h"
#include "../scene.h"
using namespace motorcar;

Drawable::Drawable(SceneGraphNode *parent, const glm::mat4 &transform)
    : VirtualNode(parent, transform)
    , m_valid(true)
{
}




void Drawable::traverseNode(Scene *scene, long deltaMillis)
{
    VirtualNode::traverseNode(scene, deltaMillis);
    this->draw(scene);
}

void Drawable::draw(Scene *scene)
{
    if(valid()){
        for(Display *display : scene->displays()){
            display->renderDrawable(this);
        }
    }

}




bool Drawable::valid() const
{
    return m_valid;
}

void Drawable::setValid(bool valid)
{
    m_valid = valid;
}


