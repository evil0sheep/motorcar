#include "spatialpointingdevice.h"
#include "../scene.h"

using namespace motorcar;
SpatialPointingDevice::SpatialPointingDevice(Seat *seat, PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
    ,m_seat(seat)
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

    if(intersection != NULL){
        mouseEvent(MouseEvent::Event::MOVE, MouseEvent::Button::NONE);

        WaylandSurfaceNode *cursor = scene->cursorNode();
        if(cursor){
           WaylandSurfaceNode *surfaceNode = intersection->surfaceNode;
           glm::vec3 position = glm::vec3(surfaceNode->surfaceTransform() *
                                          glm::vec4((intersection->surfaceLocalCoordinates +
                                                     (glm::vec2(cursor->surface()->size())/2.0f) - glm::vec2(scene->cursorHotspot())) /
                                                    glm::vec2(surfaceNode->surface()->size()),0.01,1));

           cursor->setParentNode(surfaceNode);
           cursor->setTransform(glm::translate(glm::mat4(), position));
           cursor->setValid(true);
        }
    }else{
        WaylandSurfaceNode *cursor = scene->cursorNode();
        if(cursor){
            cursor->setValid(false);
        }
    }

    if(m_grabbedSurfaceNode != NULL){
        m_grabbedSurfaceNode->setTransform( m_grabbedSurfaceNode->parentNode()->inverseWorldTransform() * this->worldTransform() * m_grabbedSurfaceNodeTransform);
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
        m_grabbedSurfaceNode->setTransform(m_grabbedSurfaceNode->parentNode()->inverseWorldTransform() * this->worldTransform() * m_grabbedSurfaceNodeTransform);
        m_grabbedSurfaceNode = NULL;
    }
}



void SpatialPointingDevice::mouseEvent(MouseEvent::Event event, MouseEvent::Button button)
{
    if(m_latestIntersection != NULL){
        //std::cout << "found surface to send mouse event to" << std::endl;
        WaylandSurface *surface = m_latestIntersection->surfaceNode->surface();
        surface->sendEvent(MouseEvent(event, button, m_latestIntersection->surfaceLocalCoordinates, m_seat));
    }else{
        //std::cout << "could not find surface to send mouse event to" << std::endl;
    }
}
Seat *SpatialPointingDevice::seat() const
{
    return m_seat;
}

void SpatialPointingDevice::setSeat(Seat *seat)
{
    m_seat = seat;
}




bool SpatialPointingDevice::rightMouseDown() const
{
    return m_rightMouseDown;
}

void SpatialPointingDevice::setRightMouseDown(bool rightMouseDown)
{
    if(rightMouseDown != m_rightMouseDown){
        if(rightMouseDown){
            mouseEvent(MouseEvent::Event::BUTTON_PRESS, MouseEvent::Button::RIGHT);
        }else{
            mouseEvent(MouseEvent::Event::BUTTON_RELEASE, MouseEvent::Button::RIGHT);
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
            mouseEvent(MouseEvent::Event::BUTTON_PRESS, MouseEvent::Button::LEFT);
        }else{
            mouseEvent(MouseEvent::Event::BUTTON_RELEASE, MouseEvent::Button::LEFT);
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
            mouseEvent(MouseEvent::Event::BUTTON_PRESS, MouseEvent::Button::MIDDLE);
        }else{
            mouseEvent(MouseEvent::Event::BUTTON_RELEASE, MouseEvent::Button::MIDDLE);
        }
    }
    m_middleMouseDown = middleMouseDown;
}




