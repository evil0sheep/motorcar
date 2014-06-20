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
#ifndef RENDERTOTEXTUREDISPLAY_H
#define RENDERTOTEXTUREDISPLAY_H

#include "display.h"
#include "../../../gl/openglshader.h"

namespace motorcar{
class RenderToTextureDisplay : public Display
{
public:
    RenderToTextureDisplay(float scale, glm::vec4 distortionK, OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~RenderToTextureDisplay();

    //inherited from Display
    virtual void prepareForDraw() override;
    virtual void finishDraw() override;


    //inherited from Display, apply scaling factor to base class output
    virtual glm::ivec2 size() override;
    virtual glm::vec2 dimensions() const override;

    virtual GLuint activeFrameBuffer() const override;
    virtual GLuint depthBufferTexture() const override;
private:
    float m_scale;
    glm::vec4 m_distortionK;
    GLuint m_frameBuffer, m_colorBufferTexture, m_depthBufferTexture, m_surfaceTextureCoordinates, m_surfaceVertexCoordinates;
    //shaders
    OpenGLShader *m_distortionShader;

    //shader variable handles
    GLint h_aPosition_distortion, h_aTexCoord_distortion, h_uDistortionK, h_uLenseCenter, h_uViewportParams, h_uScaleFactor;


};
}


#endif // RENDERTOTEXTUREDISPLAY_H
