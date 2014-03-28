#include "windowmanager.h"

using namespace motorcar;
WindowManager::WindowManager(Scene *scene)
    :m_numSurfacesMapped(0)
    ,m_scene(scene)
{
}

WaylandSurfaceNode *WindowManager::createSurface(WaylandSurface *surface)
{
    WaylandSurfaceNode *surfaceNode = new WaylandSurfaceNode(surface, this->scene());
    m_surfaceMap.insert(std::pair<WaylandSurface *, motorcar::WaylandSurfaceNode *>(surface, surfaceNode));
}

void WindowManager::destroySurface(WaylandSurface *surface)
{
    WaylandSurfaceNode *surfaceNode = this->getSurfaceNode(surface);
    if(surfaceNode != NULL){

        std::vector<motorcar::SceneGraphNode *> subtreeNodes = surfaceNode->nodesInSubtree();
        for(motorcar::SceneGraphNode *node : subtreeNodes){
            motorcar::WaylandSurfaceNode *subtreeSurfaceNode = dynamic_cast<motorcar::WaylandSurfaceNode *>(node);
            if(subtreeSurfaceNode != NULL){
                WaylandSurface *subtreeSurface = subtreeSurfaceNode->surface();

                    std::map<WaylandSurface *, motorcar::WaylandSurfaceNode *>::iterator it = m_surfaceMap.find(subtreeSurface);
                    if (it != m_surfaceMap.end()){
                        std::cout << "nulling surfaceNode pointer: " << it->second  << " in surface map" <<std::endl;
                        it->second = NULL;
                    }

            }
        }
        m_surfaceMap.erase (surface);

        std::cout << "attempting to delete surfaceNode pointer " << surfaceNode <<std::endl;

        delete surfaceNode;


        this->setMouseFocus(NULL);
        this->setKeyboardFocus(NULL);


    }
}


WaylandSurfaceNode *WindowManager::mapSurface(motorcar::WaylandSurface *surface, WaylandSurface::SurfaceType surfaceType)
{
    glm::mat4 transform;
    SceneGraphNode *parentNode;

    int type = static_cast<int>(surfaceType);
    float popupZOffset = 0.05;


    if(type == WaylandSurface::SurfaceType::TOPLEVEL){
        parentNode = this->scene();
        transform = glm::mat4(1)
                      //  * glm::rotate(glm::mat4(1), -90.f, glm::vec3(0, 1, 0))
                        * glm::translate(glm::mat4(1), glm::vec3(0, 0 ,1.25f))
                        * glm::rotate(glm::mat4(1), (-1 +  m_numSurfacesMapped % 3) * 30.f, glm::vec3(0, -1, 0))
                    * glm::rotate(glm::mat4(1),  (-1 + m_numSurfacesMapped / 3) * 30.f, glm::vec3(-1, 0, 0))
                    * glm::translate(glm::mat4(1), glm::vec3(0,0.0,-1.5f))
                    * glm::mat4(1);
        m_numSurfacesMapped ++;
    }else if(type == WaylandSurface::SurfaceType::POPUP ||
             type == WaylandSurface::SurfaceType::TRANSIENT){

        WaylandSurfaceNode *parentSurfaceNode;
        if(surface->parentSurface() != NULL){
           parentSurfaceNode = this->getSurfaceNode(surface->parentSurface());
           glm::vec3 position = glm::vec3(parentSurfaceNode->surfaceTransform() *
                                          glm::vec4((glm::vec2(surface->position()) + glm::vec2(surface->size()) / 2.0f) /
                                                       glm::vec2(parentSurfaceNode->surface()->size()), popupZOffset, 1));
           std::cout << "creating popup/transient window with parent " << parentSurfaceNode << " at position:" << std::endl;
           motorcar::Geometry::printVector(position);
           transform = glm::translate(glm::mat4(), position);
           parentNode = parentSurfaceNode;
        }else{
            std::cout << "WARNING: creating popup/transient window with no parent " << std::endl;
             parentNode = this->scene();
             transform = glm::mat4();
        }





    }else{
        transform = glm::mat4();
        parentNode = this->scene();
    }

    //WaylandSurfaceNode *surfaceNode = new WaylandSurfaceNode(surface, parentNode, transform);

    WaylandSurfaceNode *surfaceNode = getSurfaceNode(surface);

    if(surfaceNode != NULL){
        surfaceNode->surface()->setType(surfaceType);
        surfaceNode->setParentNode(parentNode);
        surfaceNode->setTransform(transform);
    }else{
        surfaceNode = new WaylandSurfaceNode(surface, parentNode, transform);
        m_surfaceMap.insert(std::pair<WaylandSurface *, motorcar::WaylandSurfaceNode *>(surface, surfaceNode));
    }

    if(type == WaylandSurface::SurfaceType::POPUP || type == WaylandSurface::SurfaceType::TOPLEVEL){
        this->setMouseFocus(surfaceNode);
    }

    surfaceNode->setMapped(true);
    return surfaceNode;
}

void WindowManager::unmapSurface(WaylandSurface *surface)
{
    WaylandSurfaceNode *surfaceNode = this->getSurfaceNode(surface);
    if(surfaceNode != NULL){
        surfaceNode->setMapped(false);
    }else{
        std::cout << "Warning: surface unmapped but doesnt have associated surfaceNode" <<std::endl;
    }
}

void WindowManager::sendEvent(const Event &event)
{
    switch(event.type()){
    case Event::EventType::MOUSE:
        this->mouseFocus()->surface()->sendEvent(event);
        break;
    case Event::EventType::KEYBOARD:
        this->keyboardFocus()->surface()->sendEvent(event);
        break;
    default:
        break;
    }
}

WaylandSurfaceNode *WindowManager::getSurfaceNode(WaylandSurface *surface) const
{
    if(surface != NULL && m_surfaceMap.count(surface)){
        return m_surfaceMap.find(surface)->second;
    }else{
        return NULL;
    }

}
WaylandSurfaceNode *WindowManager::mouseFocus() const
{
    return m_mouseFocus;
}

void WindowManager::setMouseFocus(WaylandSurfaceNode *mouseFocus)
{
    m_mouseFocus = mouseFocus;
}
WaylandSurfaceNode *WindowManager::keyboardFocus() const
{
    return m_keyboardFocus;
}

void WindowManager::setKeyboardFocus(WaylandSurfaceNode *keyboardFocus)
{
    m_keyboardFocus = keyboardFocus;
}
Scene *WindowManager::scene() const
{
    return m_scene;
}

void WindowManager::setScene(Scene *scene)
{
    m_scene = scene;
}






