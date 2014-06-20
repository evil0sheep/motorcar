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
#ifndef COMPOSITOR_H
#define COMPOSITOR_H
#include "scenegraph/output/display/display.h"
#include "gl/openglcontext.h"
#include "wayland-server.h"
#include "wayland-server-protocol.h"

namespace motorcar {
///This class handles only the invoking the draw calls on the scenegraph needed to display its contents
/*Classes implementing this interface are responsible for implementing the main draw loop of the application
 * and providing OpenGL contexts that Display classes can enable before drawing the scenegraph. */
class Compositor
{
public:
    virtual ~Compositor();

    ///starts the compositor main draw loop
    /*Starts the compositor and blocks until the compositor (usually another thread) has finished,
     * returning the exit code of the compositor thread*/
    virtual int start() = 0;

    ///gets the OpenGL context in which the compositing is taking place
    /*this context is used by display classes to allow them to do all of the drawing in the correct context.
     * eventually this should be capable of returning multiple contexts, or perhaps the contexts should be created externally and passed in*/
    virtual OpenGLContext *getContext() = 0;

//    void addDisplay(Display *display);
//    std::vector<Display *> displays() const;

    Display *display() const;
    void setDisplay(Display *display);

    virtual struct wl_display *wlDisplay() = 0;


    virtual WaylandSurface *getSurfaceFromResource(struct wl_resource *resource) = 0;

private:
    //    std::vector<Display *> m_displays;
    Display *m_display;
};
}


#endif // COMPOSITOR_H
