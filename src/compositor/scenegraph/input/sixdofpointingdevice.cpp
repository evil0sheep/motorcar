/****************************************************************************
**This file is part of the Motorcar 3D windowing framework
**
**
**Copyright (C) 2014 Forrest Reiling
**
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/
#include <scenegraph/input/sixdofpointingdevice.h>
#include <scenegraph/scene.h>
#include <compositor.h>
#include <stdint.h>


using namespace motorcar;
SixDOFPointingDevice::SixDOFPointingDevice(Seat *seat, PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
    ,m_seat(seat)
    ,m_latestIntersection(NULL)
    ,m_leftMouseDown(false)
    ,m_rightMouseDown(false)
    ,m_middleMouseDown(false)
    ,m_grabbedSurfaceNode(NULL)
    ,m_grabbedSurfaceNodeTransform()
    ,m_sixDofFocus(NULL)
{ 
    float vertices[]= {
        -0.05, 0, 0,
        0.05, 0, 0,
        0, -0.05, 0,
        0, 0.05, 0,
        0, 0, -1,
        0, 0, 0.05,


    };
    new WireframeNode(vertices, 3, glm::vec3(0.25), this);

    //new WireframeNode(vertices, 3, glm::vec3(0,1,0), this, glm::rotate(glm::mat4(), -45.f, glm::vec3(1,0,0)));


    m_global = wl_global_create(scene()->compositor()->wlDisplay(),
                     &motorcar_six_dof_pointer_interface,
                      motorcar_six_dof_pointer_interface.version,
                     this,
                     SixDOFPointingDevice::bind_func);

    wl_array_init(&m_positionArray);
    wl_array_init(&m_orientationArray);

    wl_array_add(&m_positionArray, sizeof(glm::vec3));
    wl_array_add(&m_orientationArray, sizeof(glm::mat3));

}

void SixDOFPointingDevice::handleFrameBegin(Scene *scene)
{std::map<WaylandSurface *, WaylandSurfaceNode *> m_surfaceMap;
    PhysicalNode::handleFrameBegin(scene);

    Geometry::Ray ray = Geometry::Ray(glm::vec3(0,0,0), glm::vec3(0,0,-1)).transform(worldTransform());

    Geometry::RaySurfaceIntersection *intersection = scene->intersectWithSurfaces(ray);

    this->m_latestIntersection = intersection;

    if(intersection != NULL){
        WaylandSurfaceNode *surfaceNode = intersection->surfaceNode;


        mouseEvent(MouseEvent::Event::MOVE, MouseEvent::Button::NONE);

        WaylandSurfaceNode *cursor = m_seat->pointer()->cursorNode();
        if(cursor){
           glm::vec3 position = glm::vec3(surfaceNode->surfaceTransform() *
                                          glm::vec4((intersection->surfaceLocalCoordinates +
                                                     (glm::vec2(cursor->surface()->size())/2.0f) - glm::vec2(m_seat->pointer()->cursorHotspot())) /
                                                    glm::vec2(surfaceNode->surface()->size()),0.01,1));

           cursor->setParentNode(surfaceNode);
           cursor->setTransform(glm::translate(glm::mat4(), position));
           cursor->setVisible(true);
        }
    }else{
        WaylandSurfaceNode *cursor = m_seat->pointer()->cursorNode();
        if(cursor){
            cursor->setVisible(false);
        }
        if(m_sixDofFocus != NULL){
            sixDofPointerEvent(m_sixDofFocus, SixDofEvent(MouseEvent::Event::LEAVE,
                                                          MouseEvent::Button::NONE,
                                                          m_seat, this->worldTransform()));
            m_sixDofFocus = NULL;
        }

    }

    if(m_grabbedSurfaceNode != NULL){
        m_grabbedSurfaceNode->setTransform( m_grabbedSurfaceNode->parentNode()->inverseWorldTransform() * this->worldTransform() * m_grabbedSurfaceNodeTransform);
    }
}






void SixDOFPointingDevice::grabSurfaceUnderCursor()
{
    //std::cout << "attempting to grab surface" <<std::endl;
    if(m_grabbedSurfaceNode == NULL && m_latestIntersection != NULL){
        m_grabbedSurfaceNode =  m_latestIntersection->surfaceNode;
        //m_grabbedSurfaceParent = node->parentNode();
        //node->setParentNode(this);
        m_grabbedSurfaceNodeTransform = this->inverseWorldTransform() * m_grabbedSurfaceNode->parentNode()->worldTransform() * m_grabbedSurfaceNode->transform();

    }
}

void SixDOFPointingDevice::releaseGrabbedSurface()
{
    //std::cout << "attempting to release grabbed surface" <<std::endl;
    if(m_grabbedSurfaceNode != NULL){
        //m_grabbedSurface->setParentNode(m_grabbedSurfaceParent);
        m_grabbedSurfaceNode->setTransform(m_grabbedSurfaceNode->parentNode()->inverseWorldTransform() * this->worldTransform() * m_grabbedSurfaceNodeTransform);
        m_grabbedSurfaceNode = NULL;
    }
}



void SixDOFPointingDevice::mouseEvent(MouseEvent::Event event, MouseEvent::Button button)
{
    if(m_latestIntersection != NULL){
        //std::cout << "found surface to send mouse event to" << std::endl;
        WaylandSurface *surface = m_latestIntersection->surfaceNode->surface();

        if(surface->isMotorcarSurface()){
            MotorcarSurfaceNode *mcsn = static_cast<MotorcarSurfaceNode *>(m_latestIntersection->surfaceNode);

            if(m_sixDofFocus != mcsn){
                if(m_sixDofFocus != NULL){
                    sixDofPointerEvent(m_sixDofFocus, SixDofEvent(MouseEvent::Event::LEAVE,
                                                                  MouseEvent::Button::NONE,
                                                                  m_seat, this->worldTransform()));
                }
                m_sixDofFocus = mcsn;
                sixDofPointerEvent(m_sixDofFocus, SixDofEvent(MouseEvent::Event::ENTER,
                                                              MouseEvent::Button::NONE,
                                                              m_seat, this->worldTransform()));

            }
            sixDofPointerEvent(m_sixDofFocus, SixDofEvent(event, button, m_seat, this->worldTransform()));
        }else{
            surface->sendEvent(MouseEvent(event, button, m_latestIntersection->surfaceLocalCoordinates, m_seat));

            if(m_sixDofFocus != NULL){
                sixDofPointerEvent(m_sixDofFocus, SixDofEvent(MouseEvent::Event::LEAVE,
                                                              MouseEvent::Button::NONE,
                                                              m_seat, this->worldTransform()));
                m_sixDofFocus = NULL;
            }
        }
    }else{
        //std::cout << "could not find surface to send mouse event to" << std::endl;
    }
}



void SixDOFPointingDevice::sixDofPointerEvent(MotorcarSurfaceNode *surfaceNode, SixDofEvent event)
{

    wl_resource *motorcarSurfaceResource = surfaceNode->resource();
    wl_resource *sixDofResource = this->resourceForClient(motorcarSurfaceResource->client);

    glm::mat4 trans = event.transform();
    glm::mat3 orientation = glm::mat3(trans);
    //normalize orientation basis vectors to eliminate scale component of transform
    orientation = glm::mat3(glm::normalize(orientation[0]),
            glm::normalize(orientation[1]),
            glm::normalize(orientation[2]));
    glm::vec3 position = glm::vec3(trans[3]); //4th column is position


    std::memcpy(m_positionArray.data, glm::value_ptr(position), m_positionArray.size);
    std::memcpy(m_orientationArray.data, glm::value_ptr(orientation), m_orientationArray.size);


    uint32_t time = this->scene()->currentTimestampMillis();
    uint32_t button;
    uint32_t button_state;
    uint32_t serial = 0;

//    motorcar_six_dof_pointer_send_enter(sixDofResource, serial, motorcarSurfaceResource, m_positionArray, m_orientationArray);

//    motorcar_six_dof_pointer_send_leave(sixDofResource, serial, motorcarSurfaceResource);

    switch(event.event()){
    case(MouseEvent::Event::BUTTON_PRESS):
    case(MouseEvent::Event::BUTTON_RELEASE):
        button_state = (event.event() == MouseEvent::Event::BUTTON_PRESS) ? WL_POINTER_BUTTON_STATE_PRESSED : WL_POINTER_BUTTON_STATE_RELEASED;
        button = event.button();
        motorcar_six_dof_pointer_send_button(sixDofResource, serial, time, button, button_state);
        break;
    case(MouseEvent::Event::MOVE):
        motorcar_six_dof_pointer_send_motion(sixDofResource, time, &m_positionArray, &m_orientationArray);
        break;    
    case(MouseEvent::Event::ENTER):
        motorcar_six_dof_pointer_send_enter(sixDofResource, serial, motorcarSurfaceResource, &m_positionArray, &m_orientationArray);
        break;
    case(MouseEvent::Event::LEAVE):
        motorcar_six_dof_pointer_send_leave(sixDofResource, serial, motorcarSurfaceResource);
        break;
    }










}


Seat *SixDOFPointingDevice::seat() const
{
    return m_seat;
}

void SixDOFPointingDevice::setSeat(Seat *seat)
{
    m_seat = seat;
}






bool SixDOFPointingDevice::rightMouseDown() const
{
    return m_rightMouseDown;
}

void SixDOFPointingDevice::setRightMouseDown(bool rightMouseDown)
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

bool SixDOFPointingDevice::leftMouseDown() const
{
    return m_leftMouseDown;
}

void SixDOFPointingDevice::setLeftMouseDown(bool leftMouseDown)
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

bool SixDOFPointingDevice::middleMouseDown() const
{
    return m_middleMouseDown;
}

void SixDOFPointingDevice::setMiddleMouseDown(bool middleMouseDown)
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


void SixDOFPointingDevice::destroy_func(wl_resource *resource)
{
    SixDOFPointingDevice *device = static_cast<SixDOFPointingDevice *>(resource->data);
    //viewpoint->m_resources.
    device->m_resourcesMap.erase (resource->client);
}

void SixDOFPointingDevice::bind_func(struct wl_client *client, void *data,
                          uint32_t version, uint32_t id)
{
    SixDOFPointingDevice *device = static_cast<SixDOFPointingDevice *>(data);
    std::cout << "client bound spatial pointing device global: " << device << std::endl;
    struct wl_resource *resource = wl_resource_create(client, &motorcar_six_dof_pointer_interface, version, id);
    wl_resource_set_implementation(resource,NULL, data, SixDOFPointingDevice::destroy_func);
    device->m_resourcesMap.insert(std::pair<struct wl_client *, struct wl_resource *>(client, resource));
}

wl_resource *SixDOFPointingDevice::resourceForClient(wl_client *client)
{
    if(client != NULL && m_resourcesMap.count(client)){
        return m_resourcesMap.find(client)->second;
    }else{
        return NULL;
    }
}



