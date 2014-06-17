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
#ifndef SPATIALMOUSE_H
#define SPATIALMOUSE_H

#include <glm/gtc/type_ptr.hpp>


#include "../physicalnode.h"
#include "../output/wireframenode.h"
#include "../output/wayland/waylandsurfacenode.h"
#include "../output/wayland/depthcompositedsurfacenode.h"
#include "../../wayland/input/seat.h"
#include "../../wayland/input/pointer.h"


#include "wayland-server.h"
#include "wayland-server-protocol.h"
#include "motorcar-server-protocol.h"

namespace motorcar {
class SixDOFPointingDevice : public PhysicalNode
{
public:
    SixDOFPointingDevice(Seat *seat, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~SixDOFPointingDevice() {}


    ///updates the current device state and moves any attaches surfaces
    virtual void handleFrameBegin(Scene *scene) override;

    bool leftMouseDown() const;
    void setLeftMouseDown(bool leftMouseDown);

    bool rightMouseDown() const;
    void setRightMouseDown(bool rightMouseDown);

    bool middleMouseDown() const;
    void setMiddleMouseDown(bool middleMouseDown);



    void grabSurfaceUnderCursor();
    void releaseGrabbedSurface();



    Seat *seat() const;
    void setSeat(Seat *seat);

    struct wl_resource *resourceForClient(struct wl_client *client);

private:

    void mouseEvent(MouseEvent::Event event, MouseEvent::Button button);
    void sixDofPointerEvent(MotorcarSurfaceNode *surfaceNode, SixDofEvent event);

    Geometry::RaySurfaceIntersection *m_latestIntersection;
    bool m_leftMouseDown, m_rightMouseDown, m_middleMouseDown;

    WaylandSurfaceNode *m_grabbedSurfaceNode;
    glm::mat4 m_grabbedSurfaceNodeTransform;

    Seat *m_seat;


    struct motorcar_3d_pointer *m_pointerHandle;
    struct wl_global *m_global;
    std::map<struct wl_client *, struct wl_resource *> m_resourcesMap;


    static void destroy_func(struct wl_resource *resource);

    static void bind_func(struct wl_client *client, void *data,
                          uint32_t version, uint32_t id);

    struct wl_array m_positionArray, m_orientationArray;

    MotorcarSurfaceNode *m_sixDofFocus;

};
}


#endif // SPATIALMOUSE_H
