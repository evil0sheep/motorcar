#include "scene.h"
#include "../windowmanager.h"

using namespace motorcar;

Scene::Scene()
    :PhysicalNode()
    ,m_windowManager(NULL)
    ,m_trash(NULL)
    ,m_currentTimestampMillis(0)
    ,m_lastTimestepMillis(0)
    ,m_activeDisplay(NULL)
{
}


Scene::~Scene()
{
    delete m_windowManager;
}




//void Scene::draw(long deltaMillis)
//{

//    for(Display *display : displays()){
//        display->prepareForDraw();
//    }
//    this->traverseSceneGraph(this, deltaMillis);
//    for(Display *display : displays()){
//        display->finishDraw();
//    }
//}

Scene *Scene::scene()
{
    return this;
}

void Scene::prepareForFrame(long timeStampMillis)
{
    this->setCurrentTimestampMillis(timeStampMillis);
    this->mapOntoSubTree(&SceneGraphNode::handleFrameBegin, this);
    for(Display *display : displays()){
        for(ViewPoint *viewpoint : display->viewpoints()){
            viewpoint->updateViewMatrix();
        }
    }

}

void Scene::drawFrame()
{
    for(Display * display : this->displays()){
        this->setActiveDisplay(display);
        display->prepareForDraw();
        this->mapOntoSubTree(&SceneGraphNode::handleFrameDraw, this);
        display->finishDraw();

    }


}

void Scene::finishFrame()
{
    this->mapOntoSubTree(&SceneGraphNode::handleFrameEnd, this);

    int error = glGetError();
    if(error != GL_NO_ERROR){
        std::cout <<  "OpenGL Error from frame: " << error <<std::endl;
    }

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
long Scene::currentTimestampMillis() const
{
    return m_currentTimestampMillis;
}

void Scene::setCurrentTimestampMillis(long currentTimestampMillis)
{
    m_lastTimestepMillis = m_currentTimestampMillis;
    m_currentTimestampMillis = currentTimestampMillis;

}

long Scene::latestTimestampChange()
{
    return m_currentTimestampMillis - m_lastTimestepMillis;
}
Display *Scene::activeDisplay() const
{
    return m_activeDisplay;
}

void Scene::setActiveDisplay(Display *activeDisplay)
{
    m_activeDisplay = activeDisplay;
}






