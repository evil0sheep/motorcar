#include "spatialpointingdevice.h"
#include "../scene.h"

using namespace motorcar;

SpatialPointingDevice::SpatialPointingDevice(PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
    ,m_latestIntersection(NULL)
    ,m_leftMouseDown(false)
    ,m_rightMouseDown(false)
    ,m_middleMouseDown(false)
    ,m_grabbedSurfaceNode(NULL)
    ,m_grabbedSurfaceNodeTransform()
{ 
    float vertices[]= {
        -0.05, 0, 0,
        0.05, 0, 0,
        0, -0.05, 0,
        0, 0.05, 0,
        0, 0, -1,
        0, 0, 0.05,


    };
    new WireframeNode(vertices, 3, glm::vec3(1,0,0), this);

    //new WireframeNode(vertices, 3, glm::vec3(0,1,0), this, glm::rotate(glm::mat4(), -45.f, glm::vec3(1,0,0)));


}

void SpatialPointingDevice::traverseNode(Scene *scene, long deltaMillis)
{
    PhysicalNode::traverseNode(scene, deltaMillis);

    Geometry::Ray ray = Geometry::Ray(glm::vec3(0,0,0), glm::vec3(0,0,-1)).transform(worldTransform());

    Geometry::RaySurfaceIntersection *intersection = scene->intersectWithSurfaces(ray);

    this->m_latestIntersection = intersection;

    if(m_latestIntersection != NULL){
        mouseEvent(WaylandSurface::MouseEvent::MOVE, WaylandSurface::MouseButton::NONE);
    }

    if(m_grabbedSurfaceNode != NULL){
        m_grabbedSurfaceNode->setTransform(this->worldTransform() * m_grabbedSurfaceNode->parentNode()->inverseWorldTransform() * m_grabbedSurfaceNodeTransform);
    }
}



void SpatialPointingDevice::grabSurfaceUnderCursor()
{
    //std::cout << "attempting to grab surface" <<std::endl;
    if(m_grabbedSurfaceNode == NULL && m_latestIntersection != NULL){
        m_grabbedSurfaceNode =  m_latestIntersection->surfaceNode;
        //m_grabbedSurfaceParent = node->parentNode();
        //node->setParentNode(this);
        m_grabbedSurfaceNodeTransform = this->inverseWorldTransform() * m_grabbedSurfaceNode->parentNode()->worldTransform() * m_grabbedSurfaceNode->transform();

    }
}

void SpatialPointingDevice::releaseGrabbedSurface()
{
    //std::cout << "attempting to release grabbed surface" <<std::endl;
    if(m_grabbedSurfaceNode != NULL){
        //m_grabbedSurface->setParentNode(m_grabbedSurfaceParent);
        m_grabbedSurfaceNode->setTransform(this->worldTransform() * m_grabbedSurfaceNode->parentNode()->inverseWorldTransform() * m_grabbedSurfaceNodeTransform);
        m_grabbedSurfaceNode = NULL;
    }
}



void SpatialPointingDevice::mouseEvent(WaylandSurface::MouseEvent eventType, WaylandSurface::MouseButton button)
{
    if(m_latestIntersection != NULL){
        //std::cout << "found surface to send mouse event to" << std::endl;
        WaylandSurface *surface = m_latestIntersection->surfaceNode->surface();
        surface->sendMouseEvent(eventType, button, m_latestIntersection->surfaceLocalCoordinates);
    }else{
        //std::cout << "could not find surface to send mouse event to" << std::endl;
    }
}



bool SpatialPointingDevice::rightMouseDown() const
{
    return m_rightMouseDown;
}

void SpatialPointingDevice::setRightMouseDown(bool rightMouseDown)
{
    if(rightMouseDown != m_rightMouseDown){
        if(rightMouseDown){
            mouseEvent(WaylandSurface::MouseEvent::BUTTON_PRESS, WaylandSurface::MouseButton::RIGHT);
        }else{
            mouseEvent(WaylandSurface::MouseEvent::BUTTON_RELEASE, WaylandSurface::MouseButton::RIGHT);
        }
    }
    m_rightMouseDown = rightMouseDown;

}

bool SpatialPointingDevice::leftMouseDown() const
{
    return m_leftMouseDown;
}

void SpatialPointingDevice::setLeftMouseDown(bool leftMouseDown)
{
    if(leftMouseDown != m_leftMouseDown){
        if(leftMouseDown){
            mouseEvent(WaylandSurface::MouseEvent::BUTTON_PRESS, WaylandSurface::MouseButton::LEFT);
        }else{
            mouseEvent(WaylandSurface::MouseEvent::BUTTON_RELEASE, WaylandSurface::MouseButton::LEFT);
        }
    }
    m_leftMouseDown = leftMouseDown;
}

bool SpatialPointingDevice::middleMouseDown() const
{
    return m_middleMouseDown;
}

void SpatialPointingDevice::setMiddleMouseDown(bool middleMouseDown)
{
    if(middleMouseDown != m_middleMouseDown){
        if(middleMouseDown){
            mouseEvent(WaylandSurface::MouseEvent::BUTTON_PRESS, WaylandSurface::MouseButton::MIDDLE);
        }else{
            mouseEvent(WaylandSurface::MouseEvent::BUTTON_RELEASE, WaylandSurface::MouseButton::MIDDLE);
        }
    }
    m_middleMouseDown = middleMouseDown;
}




