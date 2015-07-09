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
{

    h_aPosition_distortion =  glGetAttribLocation(m_distortionShader->handle(), "aPosition");
    h_aTexCoord_distortion =  safe_glGetAttribLocation(m_distortionShader->handle(), "aTexCoord");
    h_uDistortionK =  glGetUniformLocation(m_distortionShader->handle(), "uDistortionK");
    h_uLenseCenter =  glGetUniformLocation(m_distortionShader->handle(), "uLenseCenter");
    h_uViewportParams =  glGetUniformLocation(m_distortionShader->handle(), "uViewportParams");
    h_uScaleFactor = glGetUniformLocation(m_distortionShader->handle(), "uScaleFactor");

    //printOpenGLError();

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


    glm::ivec2 res = size();

    std::cout << res.x << " , " << res.y <<std::endl;


    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

    glGenTextures(1, &m_colorBufferTexture);
    glBindTexture(GL_TEXTURE_2D, m_colorBufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res.x, res.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBufferTexture, 0);

//    glGenTextures(1, &m_depthBufferTexture);
//    glBindTexture(GL_TEXTURE_2D, m_depthBufferTexture);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, res.x, res.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBufferTexture, 0);


        glGenRenderbuffers(1, &m_depthBufferTexture);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferTexture);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, res.x, res.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferTexture);

    std::cout << "Checking Distortion Framebuffer:" << std::endl;
    switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)){
            case(GL_FRAMEBUFFER_COMPLETE):
                std::cout << "Framebuffer Complete" << std::endl;
                break;
            case(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT):
                std::cout << "Framebuffer Attachment Incomplete" << std::endl;
                break;
            case(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT):
                std::cout << "Framebuffer Attachment Incomplete/Missing" << std::endl;
                break;
            case(GL_FRAMEBUFFER_UNSUPPORTED):
                std::cout << "Framebuffer Unsupported" << std::endl;
                break;
            default:
                std::cout << "Framebuffer is Incomplete for Unknown Reasons" << std::endl;
                break;
    }


    //generate new renderbuffers of correct size
    glBindFramebuffer(GL_FRAMEBUFFER, m_scratchFrameBuffer);

    glGenTextures(1, &m_scratchColorBufferTexture);
    glBindTexture(GL_TEXTURE_2D, m_scratchColorBufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res.x, res.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_scratchColorBufferTexture, 0);

    glGenTextures(1, &m_scratchDepthBufferTexture);
    glBindTexture(GL_TEXTURE_2D, m_scratchDepthBufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, res.x, res.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_scratchDepthBufferTexture, 0);

//    glBindRenderbuffer(GL_RENDERBUFFER, m_scratchColorBuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, res.x, res.y);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_scratchColorBuffer);

//    glBindRenderbuffer(GL_RENDERBUFFER, m_scratchDepthBuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, res.x, res.y);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_scratchDepthBuffer);

    std::cout << "Checking Modified Scratch Framebuffer:" << std::endl;
    switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)){
            case(GL_FRAMEBUFFER_COMPLETE):
                std::cout << "Framebuffer Complete" << std::endl;
                break;
            case(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT):
                std::cout << "Framebuffer Attachment Incomplete" << std::endl;
                break;
            case(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT):
                std::cout << "Framebuffer Attachment Incomplete/Missing" << std::endl;
                break;
            case(GL_FRAMEBUFFER_UNSUPPORTED):
                std::cout << "Framebuffer Unsupported" << std::endl;
                break;
            default:
                std::cout << "Framebuffer is Incomplete for Unknown Reasons" << std::endl;
                break;
    }


    glEnable(GL_TEXTURE_2D);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

RenderToTextureDisplay::~RenderToTextureDisplay()
{
    glDeleteFramebuffers(1, &m_frameBuffer);
    delete m_distortionShader;


}

void RenderToTextureDisplay::prepareForDraw()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
    Display::prepareForDraw();
}

void RenderToTextureDisplay::finishDraw()
{

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glUseProgram(m_distortionShader->handle());

    //std::cout << "distortion shader handle " << m_distortionShader->handle() << std::endl;

    glEnableVertexAttribArray(h_aPosition_distortion);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glVertexAttribPointer(h_aPosition_distortion, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindTexture(GL_TEXTURE_2D, m_colorBufferTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    float temp_scale = m_scale;
    m_scale = 1;




    for(ViewPoint *viewpoint : viewpoints()){

        viewpoint->viewport()->set();


        glUniform2fv(h_uLenseCenter, 1, glm::value_ptr(glm::vec2(viewpoint->centerOfFocus())));
        glUniform4fv(h_uViewportParams, 1, glm::value_ptr(viewpoint->viewport()->viewportParams()));
        glUniform4fv(h_uDistortionK, 1, glm::value_ptr(m_distortionK));
        glUniform1f(h_uScaleFactor, temp_scale);



        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    }

    m_scale = temp_scale;

    glBindTexture(GL_TEXTURE_2D, 0);


    glDisableVertexAttribArray(h_aPosition_distortion);



    glUseProgram(0);


}


glm::ivec2 RenderToTextureDisplay::size()
{
    return glm::ivec2(m_scale * glm::vec2(Display::size()));
}

glm::vec2 RenderToTextureDisplay::dimensions() const
{
    return m_scale * Display::dimensions();
}

GLuint RenderToTextureDisplay::activeFrameBuffer() const
{
    return m_frameBuffer;
}

GLuint RenderToTextureDisplay::depthBufferTexture() const
{
    return m_depthBufferTexture;
}
