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
#ifndef GLCAMERANODE_H
#define GLCAMERANODE_H
#include <geometry.h>
#include <gl/viewport.h>
#include <scenegraph/virtualnode.h>
struct wl_global;

#include <wayland-server.h>
#include <wayland-server-protocol.h>
#include <motorcar-server-protocol.h>




namespace motorcar {
class Display;
class ViewPoint : public VirtualNode
{
public:
    //centerOfProjection: center of projection in camera space, applied as a translation to the projection matrix
    ViewPoint(float near, float far, Display *display, SceneGraphNode *parent,
              glm::mat4 transform = glm::mat4(), glm::vec4 viewPortParams = glm::vec4(0,0,1,1), glm::vec3 centerOfProjection = glm::vec3(0));
    ~ViewPoint();






    void updateViewMatrix();
    void updateProjectionMatrix();
    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;


    //returns camera vertical field of view in degrees
    float fov(Display *display);

    Geometry::Ray worldRayAtDisplayPosition(float pixelX, float pixelY);

    ViewPort *viewport() const;
    void setViewport(ViewPort *viewport);

    glm::vec4 centerOfFocus() const;


    motorcar_viewpoint *viewpointHandle() const;
    void setViewpointHandle(motorcar_viewpoint *viewpointHandle);

    void sendViewMatrixToClients();
    void sendProjectionMatrixToClients();
    void sendViewPortToClients();
    void sendCurrentStateToSingleClient(wl_resource *resource);

    wl_global *global() const;
    void setGlobal(wl_global *global);

    ViewPort *clientColorViewport() const;
    void setClientColorViewport(ViewPort *clientColorViewport);

    ViewPort *clientDepthViewport() const;
    void setClientDepthViewport(ViewPort *clientDepthViewport);

    Display *display() const;

    Geometry::Rectangle *bufferGeometry() const;
    void setBufferGeometry(Geometry::Rectangle *bufferGeometry);

private:
    Display *m_display;

    float near, far;
    ViewPort *m_viewport;
    ViewPort *m_clientColorViewport;
    ViewPort *m_clientDepthViewport;


    //center of projection information
    glm::vec4 m_centerOfFocus;
    glm::mat4 m_COFTransform;
    Geometry::Rectangle *m_bufferGeometry;


    //cached matrices
    glm::mat4 m_viewMatrix, m_projectionMatrix, m_viewProjectionMatrix;

    struct motorcar_viewpoint *m_viewpointHandle;
    struct wl_global *m_global;
    std::vector<struct wl_resource*> m_resources;

    static void destroy_func(struct wl_resource *resource);

    static void bind_func(struct wl_client *client, void *data,
                          uint32_t version, uint32_t id);

    struct wl_array m_viewArray, m_projectionArray;



};
}



#endif // GLCAMERANODE_H
