#include "scene.h"

using namespace motorcar;

Scene::Scene()
    :PhysicalNode()
    ,m_cursorNode(NULL)
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
        display->prepareForDraw();
    }
    this->traverseSceneGraph(this, deltaMillis);
    for(Display *display : displays()){
        display->finishDraw();
    }
}



WaylandSurfaceNode *Scene::cursorNode() const
{
    return m_cursorNode;
}

void Scene::setCursorNode(WaylandSurfaceNode *cursorNode)
{
    m_cursorNode = cursorNode;
}

glm::ivec2 Scene::cursorHotspot() const
{
    return m_cursorHotspot;
}

void Scene::setCursorHotspot(const glm::ivec2 &cursorHotspot)
{
    m_cursorHotspot = cursorHotspot;
}
