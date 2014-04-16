#include "drawable.h"
#include "../scene.h"
using namespace motorcar;

Drawable::Drawable(SceneGraphNode *parent, const glm::mat4 &transform)
    : VirtualNode(parent, transform)
{
}

void Drawable::handleFrameDraw(Scene *scene)
{
    VirtualNode::handleFrameDraw(scene);
    if(visible()){
        this->draw(scene, scene->activeDisplay());
    }
}

bool Drawable::visible() const
{
    return m_visible;
}

void Drawable::setVisible(bool visible)
{
    m_visible = visible;
}






