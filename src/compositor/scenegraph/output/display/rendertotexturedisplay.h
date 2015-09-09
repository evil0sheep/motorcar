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

#include <scenegraph/output/display/display.h>
#include <gl/openglshader.h>

namespace motorcar{
class RenderToTextureDisplay : public Display
{
public:

    //Motorcar equivilent of ovrDistortionVertex
    typedef struct DistortionVertex_
    {
        glm::vec2 ScreenPosNDC;    ///< [-1,+1],[-1,+1] over the entire framebuffer.
        float       TimeWarpFactor;  ///< Lerp factor between time-warp matrices. Can be encoded in Pos.z.
        float       VignetteFactor;  ///< Vignette fade factor. Can be encoded in Pos.w.
        glm::vec2 TanEyeAnglesR;   ///< The tangents of the horizontal and vertical eye angles for the red channel.
        glm::vec2 TanEyeAnglesG;   ///< The tangents of the horizontal and vertical eye angles for the green channel.
        glm::vec2 TanEyeAnglesB;   ///< The tangents of the horizontal and vertical eye angles for the blue channel.
    } DistortionVertex;


    //Motorcar equivilent of ovrDistortionMesh
    typedef struct DistortionMesh_
    {
        DistortionVertex*   pVertexData; ///< The distortion vertices representing each point in the mesh.
        unsigned short*      pIndexData;  ///< Indices for connecting the mesh vertices into polygons.
        unsigned int         VertexCount; ///< The number of vertices in the mesh.
        unsigned int         IndexCount;  ///< The number of indices in the mesh.
    } DistortionMesh; 

    //used by interface for Oculus 0.1.3 sdk
    RenderToTextureDisplay(float scale, glm::vec4 distortionK, OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    //used by interface for Oculus 0.5.0.1 sdk
    RenderToTextureDisplay(OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~RenderToTextureDisplay();

    //inherited from Display
    virtual void prepareForDraw() override;
    virtual void finishDraw() override;


    //inherited from Display, apply scaling factor to base class output
    virtual glm::ivec2 size() override;
    virtual glm::vec2 dimensions() const override;



    virtual GLuint activeFrameBuffer() const override;
    virtual GLuint depthBufferTexture() const override;
protected:
        //sets size of intermediate render target
        void setRenderTargetSize(glm::ivec2 size);
        void setDistortionMesh(DistortionMesh distortionMesh[2]);
        
private:
    //used by interface for Oculus 0.1.3 sdk
    float m_scale;
    glm::vec4 m_distortionK;
    GLuint m_surfaceTextureCoordinates, m_surfaceVertexCoordinates;
    GLint h_aPosition_distortion, h_aTexCoord_distortion, h_uDistortionK, h_uLenseCenter, h_uViewportParams, h_uScaleFactor;


    //used by interface for Oculus 0.5.0.1 sdk
    DistortionMesh m_distortionMesh[2];
    GLuint m_distortionVertexBuffers[2], m_distortionIndexBuffers[2];
    // GLint h_aPos_Mesh, h_aTexCoord_distortion, h_uDistortionK, h_uLenseCenter, h_uViewportParams, h_uScaleFactor;

    //common
    bool m_renderingToTexture, m_usingDistortionMesh;
    glm::ivec2 m_renderTargetSize;
    GLuint m_frameBuffer, m_colorBufferTexture, m_depthBufferTexture;

    
    //shaders
    OpenGLShader *m_distortionShader;

    


};
}


#endif // RENDERTOTEXTUREDISPLAY_H
