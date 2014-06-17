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
#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "../virtualnode.h"
#include "viewpoint.h"

namespace motorcar {
class Drawable : public VirtualNode
{
public:
    Drawable(SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~Drawable(){}

    ///Draw this node for the current display
    /* this method should contain all the openGL code needed to draw the node,
     * implementations can assume that the correct context is current and the
     * correct framebuffer is bound. If the framebuffer needs to be unbound
     * the implementation must tell the display to rebind its framebuffer
     * before drawing into it, do not assume drawing into the default
     * framebuffer is safe*/
    virtual void draw(Scene *scene, Display *display) = 0;

    ///Gets the active display from the scene and calls draw on it
    virtual void handleFrameDraw(Scene *scene) override;

    bool visible() const;
    void setVisible(bool visible);

private:
    bool m_visible;

};
}


#endif // DRAWABLE_H
