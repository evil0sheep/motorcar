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
#include <scenegraph/output/display/rendertotexturedisplay.h>

#include <gl/GLSLHelper.h>
using namespace motorcar;



RenderToTextureDisplay::RenderToTextureDisplay(float scale, glm::vec4 distortionK, OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform)
    :Display(glContext, displayDimensions, parent, transform)
    ,m_scale(scale)
    ,m_distortionK(distortionK)
    ,m_distortionShader(new motorcar::OpenGLShader(SHADER_MOTORCARBARRELDISTORTION))
    ,m_frameBuffer(0)
    ,m_usingDistortionMesh(false)
{

    h_aPosition_distortion =  glGetAttribLocation(m_distortionShader->handle(), "aPosition");
    h_aTexCoord_distortion =  safe_glGetAttribLocation(m_distortionShader->handle(), "aTexCoord");
    h_uDistortionK =  glGetUniformLocation(m_distortionShader->handle(), "uDistortionK");
    h_uLenseCenter =  glGetUniformLocation(m_distortionShader->handle(), "uLenseCenter");
    h_uViewportParams =  glGetUniformLocation(m_distortionShader->handle(), "uViewportParams");
    h_uScaleFactor = glGetUniformLocation(m_distortionShader->handle(), "uScaleFactor");

    printOpenGLError();

    if(h_aPosition_distortion < 0 || h_aTexCoord_distortion < 0 || h_uDistortionK < 0 || h_uLenseCenter < 0|| h_uViewportParams < 0 || h_uScaleFactor < 0){
       std::cout << "problem with distortion shader handles: "
                 << h_aPosition_distortion
                 << ", "<< h_aTexCoord_distortion
                 << ", "<< h_uDistortionK
                 << ", "<< h_uLenseCenter
                 << ", "<< h_uViewportParams
                 << ", "<< h_uScaleFactor
                 << std::endl;
    }


    const GLfloat vertexCoordinates[] ={
       -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
       -1.0f,  1.0f, 0.0f
    };



    glGenBuffers(1, &m_surfaceTextureCoordinates);

    glGenBuffers(1, &m_surfaceVertexCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertexCoordinates, GL_STATIC_DRAW);

    setRenderTargetSize(glm::ivec2(scale * glm::vec2(glContext->defaultFramebufferSize())));
}


void RenderToTextureDisplay::setDistortionMesh(DistortionMesh distortionMesh[2])
{
    for(int i = 0; i < 2; i++){
        m_distortionMesh[i] = distortionMesh[i];
        glGenBuffers(1, &(m_distortionMesh[i].VertexBuffer));
        glGenBuffers(1, &(m_distortionMesh[i].IndexBuffer));
        glBindBuffer(GL_ARRAY_BUFFER, m_distortionMesh[i].VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_distortionMesh[i].VertexCount * sizeof(DistortionVertex), m_distortionMesh[i].VertexData, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_distortionMesh[i].IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_distortionMesh[i].IndexCount * sizeof(unsigned short), m_distortionMesh[i].IndexData, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}

RenderToTextureDisplay::RenderToTextureDisplay(OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform)
    :Display(glContext, displayDimensions, parent, transform)
    ,m_distortionShader(new motorcar::OpenGLShader(SHADER_MOTORCARMESHDISTORTION))
    ,m_frameBuffer(0)
    ,m_usingDistortionMesh(true)
{
    h_aPosition_distortion =  glGetAttribLocation(m_distortionShader->handle(), "aPosition");
    h_aTanEyeAnglesR =  glGetAttribLocation(m_distortionShader->handle(), "aTanEyeAnglesR"); 
    h_aTanEyeAnglesG =  glGetAttribLocation(m_distortionShader->handle(), "aTanEyeAnglesG"); 
    h_aTanEyeAnglesB =  glGetAttribLocation(m_distortionShader->handle(), "aTanEyeAnglesB"); 
    h_uEyeToSourceUVScale =  glGetUniformLocation(m_distortionShader->handle(), "uEyeToSourceUVScale"); 
    h_uEyeToSourceUVOffset =  glGetUniformLocation(m_distortionShader->handle(), "uEyeToSourceUVOffset");

    printOpenGLError();

    if( h_aPosition_distortion < 0 || h_aTanEyeAnglesR < 0 || h_aTanEyeAnglesG < 0 || h_aTanEyeAnglesB < 0 ||  h_uEyeToSourceUVScale < 0 || h_uEyeToSourceUVOffset < 0 ){
       std::cout << "problem with distortion shader handles: "
                 << h_aPosition_distortion
                 << ", "<< h_aTanEyeAnglesR
                 << ", "<< h_aTanEyeAnglesG
                 << ", "<< h_aTanEyeAnglesB
                 << ", "<< h_uEyeToSourceUVScale
                 << ", "<< h_uEyeToSourceUVOffset
                 << std::endl;
    }

}


RenderToTextureDisplay::~RenderToTextureDisplay()
{
    glDeleteFramebuffers(1, &m_frameBuffer);
    glDeleteTextures(1, &m_colorBufferTexture);
    glDeleteRenderbuffers(1, &m_depthBufferTexture);
    delete m_distortionShader;

    for(int i = 0; i < 2; i++){
        free(m_distortionMesh[i].VertexData);
        free(m_distortionMesh[i].IndexData);
    }

}

void RenderToTextureDisplay::prepareForDraw()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
    m_renderingToTexture = true;
    Display::prepareForDraw();
}

void RenderToTextureDisplay::finishDraw()
{

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glUseProgram(m_distortionShader->handle());

    //std::cout << "distortion shader handle " << m_distortionShader->handle() << std::endl;

    if(m_usingDistortionMesh){
        glDisable(GL_CULL_FACE);
        m_renderingToTexture = false;
        glViewport(0,0, size().x, size().y);

        glBindTexture(GL_TEXTURE_2D, m_colorBufferTexture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glEnableVertexAttribArray(h_aPosition_distortion);
        glEnableVertexAttribArray(h_aTanEyeAnglesR);
        glEnableVertexAttribArray(h_aTanEyeAnglesG);
        glEnableVertexAttribArray(h_aTanEyeAnglesB);
        for(int i=0; i<2; i++){
            glUniform2fv(h_uEyeToSourceUVScale, 1, glm::value_ptr( m_distortionMesh[i].EyeToSourceUVScale));
            glUniform2fv(h_uEyeToSourceUVOffset, 1, glm::value_ptr( m_distortionMesh[i].EyeToSourceUVOffset));
            glBindBuffer(GL_ARRAY_BUFFER, m_distortionMesh[i].VertexBuffer);

            glVertexAttribPointer(h_aPosition_distortion, 4, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), (void*)0);  //vec4 pos
            glVertexAttribPointer(h_aTanEyeAnglesR, 2, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), (void*)16); //glm::vec2 TanEyeAnglesR
            glVertexAttribPointer(h_aTanEyeAnglesG, 2, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), (void*)24); //glm::vec2 TanEyeAnglesG
            glVertexAttribPointer(h_aTanEyeAnglesB, 2, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), (void*)32); //glm::vec2 TanEyeAnglesB

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_distortionMesh[i].IndexBuffer);

            glDrawElements(GL_TRIANGLES, m_distortionMesh[i].IndexCount, GL_UNSIGNED_SHORT, (void*)0);

        }
        glDisableVertexAttribArray(h_aPosition_distortion);
        glDisableVertexAttribArray(h_aTanEyeAnglesR);
        glDisableVertexAttribArray(h_aTanEyeAnglesG);
        glDisableVertexAttribArray(h_aTanEyeAnglesB);

        glBindTexture(GL_TEXTURE_2D, 0);
        m_renderingToTexture = true;
    }else{
        glEnableVertexAttribArray(h_aPosition_distortion);
        glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
        glVertexAttribPointer(h_aPosition_distortion, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindTexture(GL_TEXTURE_2D, m_colorBufferTexture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_renderingToTexture = false;
        for(ViewPoint *viewpoint : viewpoints()){

            viewpoint->viewport()->set();
            glUniform2fv(h_uLenseCenter, 1, glm::value_ptr(glm::vec2(viewpoint->centerOfFocus())));
            glUniform4fv(h_uViewportParams, 1, glm::value_ptr(viewpoint->viewport()->viewportParams()));
            glUniform4fv(h_uDistortionK, 1, glm::value_ptr(m_distortionK));
            glUniform1f(h_uScaleFactor, m_scale);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        }
        m_renderingToTexture = true;
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisableVertexAttribArray(h_aPosition_distortion);
    }
    glUseProgram(0);


}


glm::ivec2 RenderToTextureDisplay::size()
{
    if(m_renderingToTexture){
        return m_renderTargetSize;
    }else{
        return Display::size();
    }
    

}

void RenderToTextureDisplay::setRenderTargetSize(glm::ivec2 size){
    m_renderTargetSize = size;

    createOrUpdateFBO("RenderToTextureDisplay Primary Frame Buffer", m_frameBuffer,
                        m_colorBufferTexture, true, 
                        m_depthBufferTexture, false, size);
    createOrUpdateFBO("Display Scratch Frame Buffer", m_scratchFrameBuffer,
                        m_scratchColorBufferTexture, true, 
                        m_scratchDepthBufferTexture, true, size);
}



glm::vec2 RenderToTextureDisplay::dimensions() const
{
    return Display::dimensions();
}

GLuint RenderToTextureDisplay::activeFrameBuffer() const
{
    return m_frameBuffer;
}

GLuint RenderToTextureDisplay::depthBufferTexture() const
{
    return m_depthBufferTexture;
}
