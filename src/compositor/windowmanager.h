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
#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <scenegraph/output/wayland/waylandsurfacenode.h>
#include <scenegraph/output/wayland/motorcarsurfacenode.h>
#include <scenegraph/scene.h>
#include <events/events.h>
#include <wayland/input/waylandinput.h>
#include <compositor.h>
#include <shell.h>
#include <map>

namespace motorcar{
///Handles input events and window positioning
/*This class is responsible for creating surfaceNodes for new surfaces, and for positioning them in the scene,
 *as well as delivering events to surfaces based on incoming events from other */
class WindowManager
{
public:


    WindowManager(Scene *scene, Seat *defaultSeat);
    virtual ~WindowManager();

    virtual WaylandSurfaceNode *createSurface(WaylandSurface *surface);
    virtual void destroySurface(WaylandSurface *surface);

    virtual WaylandSurfaceNode *mapSurface(WaylandSurface *surface, WaylandSurface::SurfaceType surfaceType);
    virtual void unmapSurface(WaylandSurface *surface);


    virtual void sendEvent(const Event &event);

    WaylandSurfaceNode *getSurfaceNode(WaylandSurface *surface) const;




    Scene *scene() const;
    void setScene(Scene *scene);

    Seat *defaultSeat() const;
    void setDefaultSeat(Seat *defaultSeat);

    void ensureKeyboardFocusIsValid(WaylandSurface *oldSurface);

private:

    std::map<WaylandSurface *, WaylandSurfaceNode *> m_surfaceMap;
    int m_numSurfacesMapped;
    Scene *m_scene;
    Seat *m_defaultSeat;
    Shell *m_shell;
};
}


#endif // WINDOWMANAGER_H
