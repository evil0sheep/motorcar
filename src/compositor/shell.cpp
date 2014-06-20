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
#include "shell.h"
#include "scenegraph/scene.h"
#include "compositor.h"
#include "windowmanager.h"

using namespace motorcar;



void get_motorcar_surface(struct wl_client *client,
                           struct wl_resource *resource,
                           uint32_t id,
                           struct wl_resource *surface_resource,
                          uint32_t clipping_mode,
                          uint32_t enable_depth_compositing)
{
    Shell *shell = static_cast<Shell*>(resource->data);

    WaylandSurface *surface = shell->scene()->compositor()->getSurfaceFromResource(surface_resource);

    WaylandSurface::ClippingMode clippingMode;

    switch(clipping_mode){
    case(MOTORCAR_SURFACE_CLIPPING_MODE_CUBOID):
        clippingMode = WaylandSurface::ClippingMode::CUBOID;
        break;
    case(MOTORCAR_SURFACE_CLIPPING_MODE_PORTAL):
        clippingMode = WaylandSurface::ClippingMode::PORTAL;
        break;
    default:
        clippingMode = WaylandSurface::ClippingMode::CUBOID;
        break;
    }

    //WaylandSurfaceNode * surfaceNode = shell->scene()->windowManager()->mapSurface(surface, type);

    surface->setClippingMode(clippingMode);
    surface->setIsMotorcarSurface(true);
    surface->setDepthCompositingEnabled(enable_depth_compositing!=0);

    std::cout << "depth compositing enabled = " << surface->depthCompositingEnabled() << std::endl;


    MotorcarSurfaceNode *mcsn = static_cast<MotorcarSurfaceNode *> (shell->scene()->windowManager()->createSurface(surface));

    mcsn->configureResource(client, id);


}


const static struct motorcar_shell_interface motorcarShellInterface = {
    get_motorcar_surface
};

Shell::Shell(Scene *scene)
    :m_scene(scene)
{
    m_display = scene->compositor()->wlDisplay();

    struct wl_global *global =0;

    global = wl_global_create(m_display,
                     &motorcar_shell_interface,
                      motorcar_shell_interface.version,
                     this,
                     Shell::bind_func);

    std::cout << "creating global shell object" <<std::endl;
    //struct motorcar_shell_interface *shell =
}

Shell::~Shell()
{
    //todo: destroy the shell global
}
Scene *Shell::scene() const
{
    return m_scene;
}





void Shell::bind_func(struct wl_client *client, void *data,
                      uint32_t version, uint32_t id)
{
    std::cout << "Shell Bind function Called" <<std::endl;
    struct wl_resource *resource = wl_resource_create(client, &motorcar_shell_interface, version, id);
    wl_resource_set_implementation(resource, &motorcarShellInterface, data, 0);
}
