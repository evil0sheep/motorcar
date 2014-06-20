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
#ifndef WAYLANDSURFACE_H
#define WAYLANDSURFACE_H
#include <glm/glm.hpp>
#include <GL/gl.h>
#include <events/events.h>

namespace motorcar{
class WaylandSurface
{
public:
    enum SurfaceType{
        TOPLEVEL,
        TRANSIENT,
        POPUP,
        CURSOR,
        NA
    };

    enum ClippingMode{
        NONE,
        CUBOID,
        PORTAL
    };

    WaylandSurface(SurfaceType type, bool isMotorcarSurface=false, ClippingMode clippingMode = ClippingMode::NONE, bool depthCompositingEnabled = false);
    virtual ~WaylandSurface(){}

    ///Get the texture handle for this surface
    virtual GLuint texture() = 0;
    ///Get the size of this surface in pixels
    virtual glm::ivec2 size() = 0;
    ///Set the size of this surface in pixels
    virtual void setSize(glm::ivec2 newSize) = 0;
    ///Get the position of this surface in parent surface-local coordinates
    virtual glm::ivec2 position() = 0;
    ///return the parent surface
    virtual WaylandSurface *parentSurface() = 0;
    ///do any per-frame setup required for drawing
    /*note: this is the only safe place to change framebuffers*/
    virtual void prepare() = 0;
    ///returns whether or not the surface is ready to draw
    virtual bool valid() = 0;



    virtual void sendEvent(const Event &event) = 0;


    //virtual void sendMouseWheelEvent(Qt::Orientation orientation, int delta);
    


    SurfaceType type() const;
    void setType(const SurfaceType &type);

    ClippingMode clippingMode() const;
    void setClippingMode(const WaylandSurface::ClippingMode &clippingMode);

    bool depthCompositingEnabled() const;
    void setDepthCompositingEnabled(bool depthCompositingEnabled);

    bool isMotorcarSurface() const;
    void setIsMotorcarSurface(bool isMotorcarSurface);

protected:
    SurfaceType m_type;
    ClippingMode m_clippingMode;
    bool m_depthCompositingEnabled;
    bool m_isMotorcarSurface;
};
}


#endif // WAYLANDSURFACE_H
