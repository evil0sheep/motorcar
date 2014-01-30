#include "scene.h"


using namespace motorcar;

Scene::Scene()
    :SceneGraphNode()
{

}


void Scene::notifyNodeAdded(SceneGraphNode *node)
{
}

void Scene::addDisplay(Display *display)
{
    m_displays.push_back(display);
}

std::vector<Display *> Scene::displays() const
{
    return m_displays;
}

void Scene::draw(long deltaMillis)
{
    for(Display *display : displays()){
        display->prepare();
    }
    this->traverseSceneGraph(this, deltaMillis);
}


