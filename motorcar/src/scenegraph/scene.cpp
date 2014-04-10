#include "scene.h"
#include "../windowmanager.h"

using namespace motorcar;

Scene::Scene()
    :PhysicalNode()
    ,m_windowManager(NULL)
    ,m_trash(NULL)
{
}


Scene::~Scene()
{
    delete m_windowManager;
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

Scene *Scene::scene()
{
    return this;
}




WindowManager *Scene::windowManager() const
{
    return m_windowManager;
}

void Scene::setWindowManager(WindowManager *windowManager)
{
    m_windowManager = windowManager;
}
Compositor *Scene::compositor() const
{
    return m_compositor;
}

void Scene::setCompositor(Compositor *compositor)
{
    m_compositor = compositor;
}



void Scene::addDisplay(Display *display)
{
    m_displays.push_back(display);
}

std::vector<Display *> Scene::displays() const
{
    return m_displays;
}


