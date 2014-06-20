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
#include <scenegraph/output/viewpoint.h>
#include <scenegraph/output/display/display.h>
#include <scenegraph/scene.h>
#include <compositor.h>



using namespace motorcar;

//const static struct motorcar_viewpoint_interface motorcarViewpointInterface;

ViewPoint::ViewPoint(float near, float far, Display *display, SceneGraphNode *parent, glm::mat4 transform, glm::vec4 viewPortParams, glm::vec3 centerOfProjection)
    :VirtualNode(parent, transform)
    , near(near)
    , far(far)
    , m_centerOfFocus(centerOfProjection, 1)
    , m_COFTransform(glm::translate(glm::mat4(1), centerOfProjection))
    , m_display(display)
    , m_viewport(NULL)
    , m_clientColorViewport(NULL)
    , m_clientDepthViewport(NULL)
    , m_viewpointHandle(NULL)
    , m_global(NULL)

{

    m_bufferGeometry = new Geometry::Rectangle(glm::ivec2(display->size().x, display->size().y * 2));
    glm::vec2 vpPos(viewPortParams.x, viewPortParams.y);
    glm::vec2 vpSize(viewPortParams.z, viewPortParams.w);


    m_viewport = new ViewPort(vpPos, vpSize, m_display);
    m_clientColorViewport = new ViewPort(glm::vec2(vpPos.x, vpPos.y / 2), glm::vec2(vpSize.x, vpSize.y/2), m_bufferGeometry);
    m_clientDepthViewport = new ViewPort(glm::vec2(vpPos.x, vpPos.y / 2 + vpSize.y/2), glm::vec2(vpSize.x, vpSize.y/2), m_bufferGeometry);

    std::cout << "creating viewpoint global: " << this <<std::endl;





    m_global = wl_global_create(scene()->compositor()->wlDisplay(),
                     &motorcar_viewpoint_interface,
                      motorcar_viewpoint_interface.version,
                     this,
                     ViewPoint::bind_func);

    wl_array_init(&m_viewArray);
    wl_array_init(&m_projectionArray);

    wl_array_add(&m_viewArray, sizeof(glm::mat4));
    wl_array_add(&m_projectionArray, sizeof(glm::mat4));


    updateViewMatrix();
    updateProjectionMatrix();
    sendViewPortToClients();
}

ViewPoint::~ViewPoint()
{
    delete m_viewport;
    wl_array_release(&m_viewArray);
    wl_array_release(&m_projectionArray);
}

void ViewPoint::updateViewMatrix()
{
    glm::mat4 trans = worldTransform();
    glm::vec3 center = glm::vec3(trans * glm::vec4(0, 0, 0, 1));
    glm::vec3 target = glm::vec3(trans * glm::vec4(0, 0, -1, 1));
    glm::vec3 up = glm::normalize(glm::vec3(trans * glm::vec4(0, 1, 0, 0)));
    //        glm::vec3 vec = target;
    //        qDebug() << vec.x << ", " << vec.y << ", " << vec.z;
    m_viewMatrix = glm::lookAt(center, target, up);

    this->sendViewMatrixToClients();
}

void ViewPoint::updateProjectionMatrix()
{
    m_projectionMatrix = m_COFTransform * glm::perspective(fov(display()), (m_viewport->width())/ (m_viewport->height()), near, far);

    this->sendProjectionMatrixToClients();
}


Geometry::Ray ViewPoint::worldRayAtDisplayPosition(float pixelX, float pixelY)
{

    glm::vec2 normalizedPixelPos = glm::vec2(-1, 1) * m_viewport->displayCoordsToViewportCoords(pixelX, pixelY);
    float h = (m_viewport->height()/m_viewport->width()) /2;
    float theta = glm::radians(fov(display()) / 2);
    float d = h / glm::tan(theta);

    return Geometry::Ray(glm::vec3(0), glm::normalize(glm::vec3(normalizedPixelPos, d))).transform(this->worldTransform());

}



float ViewPoint::fov(Display *display)
{

    //take camera distance to display and project it onto display normal
    glm::mat4 displayWorldTransform = display->worldTransform();
    glm::vec4 origin(0, 0, 0, 1);
    glm::vec3 cameraToDisplayVector = glm::vec3((displayWorldTransform * origin) -  (worldTransform() * origin));
    glm::vec3 displayNormal = glm::normalize(glm::vec3(displayWorldTransform * glm::vec4(0, 0, 1, 0)));
    float eyeToScreenDistance = glm::abs(glm::dot(cameraToDisplayVector, displayNormal));


    return glm::degrees(2 * atan(display->dimensions().y / (2 * eyeToScreenDistance)));
}


glm::mat4 ViewPoint::projectionMatrix() const
{
    return m_projectionMatrix;
}



glm::mat4 ViewPoint::viewMatrix() const
{
    return m_viewMatrix;
}


ViewPort *ViewPoint::viewport() const
{
    return m_viewport;
}

void ViewPoint::setViewport(ViewPort *viewport)
{
    m_viewport = viewport;
    sendViewPortToClients();
    updateProjectionMatrix();
}







Display *ViewPoint::display() const
{
    return m_display;
}
Geometry::Rectangle *ViewPoint::bufferGeometry() const
{
    return m_bufferGeometry;
}

void ViewPoint::setBufferGeometry(Geometry::Rectangle *bufferGeometry)
{
    m_bufferGeometry = bufferGeometry;
}



glm::vec4 ViewPoint::centerOfFocus() const
{
    return m_centerOfFocus;
}
motorcar_viewpoint *ViewPoint::viewpointHandle() const
{
    return m_viewpointHandle;
}

void ViewPoint::setViewpointHandle(motorcar_viewpoint *viewpointHandle)
{
    m_viewpointHandle = viewpointHandle;
}

void ViewPoint::sendViewMatrixToClients()
{
    std::memcpy(m_viewArray.data, glm::value_ptr(this->viewMatrix()), m_viewArray.size);

    for(struct wl_resource *resource : m_resources){
        motorcar_viewpoint_send_view_matrix(resource, &m_viewArray);
    }
    //
}

void ViewPoint::sendProjectionMatrixToClients()
{
    std::memcpy(m_projectionArray.data, glm::value_ptr(this->projectionMatrix()), m_projectionArray.size);
    for(struct wl_resource *resource : m_resources){
        motorcar_viewpoint_send_projection_matrix(resource, &m_projectionArray);
    }

}

void ViewPoint::sendViewPortToClients()
{
    for(struct wl_resource *resource : m_resources){
        motorcar_viewpoint_send_view_port(resource,
                                          m_clientColorViewport->offsetX(), m_clientColorViewport->offsetY(),
                                          m_clientColorViewport->width(), m_clientColorViewport->height(),
                                          m_clientDepthViewport->offsetX(), m_clientDepthViewport->offsetY(),
                                          m_clientDepthViewport->width(), m_clientDepthViewport->height());
    }
}

void ViewPoint::sendCurrentStateToSingleClient(wl_resource *resource)
{
     motorcar_viewpoint_send_view_matrix(resource, &m_viewArray);
     motorcar_viewpoint_send_projection_matrix(resource, &m_projectionArray);
     motorcar_viewpoint_send_view_port(resource,
                                       m_clientColorViewport->offsetX(), m_clientColorViewport->offsetY(),
                                       m_clientColorViewport->width(), m_clientColorViewport->height(),
                                       m_clientDepthViewport->offsetX(), m_clientDepthViewport->offsetY(),
                                       m_clientDepthViewport->width(), m_clientDepthViewport->height());
}


wl_global *ViewPoint::global() const
{
    return m_global;
}

void ViewPoint::setGlobal(wl_global *global)
{
    m_global = global;
}
ViewPort *ViewPoint::clientColorViewport() const
{
    return m_clientColorViewport;
}

void ViewPoint::setClientColorViewport(ViewPort *clientColorViewport)
{
    m_clientColorViewport = clientColorViewport;
}
ViewPort *ViewPoint::clientDepthViewport() const
{
    return m_clientDepthViewport;
}

void ViewPoint::setClientDepthViewport(ViewPort *clientDepthViewport)
{
    m_clientDepthViewport = clientDepthViewport;
}



void ViewPoint::destroy_func(wl_resource *resource)
{
    ViewPoint *viewpoint = static_cast<ViewPoint *>(resource->data);
    //viewpoint->m_resources.
    viewpoint->m_resources.erase(std::remove(viewpoint->m_resources.begin(), viewpoint->m_resources.end(), resource), viewpoint->m_resources.end());
}



glm::vec4 ViewPort::viewportParams() const
{
    return glm::vec4(m_offsetX, m_offsetY, m_width, m_height);
}
Geometry::Rectangle *ViewPort::bufferGeometry() const
{
    return m_bufferGeometry;
}

void ViewPort::setBufferGeometry(Geometry::Rectangle *bufferGeometry)
{
    m_bufferGeometry = bufferGeometry;
}




void ViewPoint::bind_func(struct wl_client *client, void *data,
                          uint32_t version, uint32_t id)
{
    ViewPoint *viewpoint = static_cast<ViewPoint *>(data);
    std::cout << "client bound viewpoint global: " << viewpoint << std::endl;
    struct wl_resource *resource = wl_resource_create(client, &motorcar_viewpoint_interface, version, id);
    wl_resource_set_implementation(resource,NULL, data, ViewPoint::destroy_func);
    viewpoint->m_resources.push_back(resource);
    viewpoint->sendCurrentStateToSingleClient(resource);
}

