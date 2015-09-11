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
#include <scenegraph/output/display/display.h>

using namespace motorcar;



Display::Display(OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
    ,m_glContext(glContext)
    ,m_dimensions(displayDimensions)
    ,m_scratchFrameBuffer(0)

{
    m_glContext->makeCurrent();
    createOrUpdateFBO("Display Scratch Frame Buffer", m_scratchFrameBuffer,
                        m_scratchColorBufferTexture, true, 
                        m_scratchDepthBufferTexture, true, size());

}

Display::~Display()
{
    glDeleteTextures(1, &m_scratchColorBufferTexture);
    glDeleteTextures(1, &m_scratchDepthBufferTexture);
    glDeleteFramebuffers(1, &m_scratchFrameBuffer);
}

void Display::prepareForDraw()
{
   //std::cout << "display size: " << this->resolution().x << ", " << this->resolution().y <<std::endl;
//    Geometry::printMatrix(worldTransform());

//    for(ViewPoint *viewpoint : viewpoints()){
//        viewpoint->updateViewMatrix();
//    }
    glContext()->makeCurrent();
    //glClearColor(.7f, .85f, 1.f, 1.0f);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearStencil(0.0);
    glStencilMask(0xFF);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc (GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
}



void Display::addViewpoint(ViewPoint *v)
{
    m_viewpoints.push_back(v);
}


std::vector<ViewPoint *> Display::viewpoints() const
{
    return m_viewpoints;
}

glm::vec2 Display::dimensions() const
{
    return m_dimensions;
}





Geometry::Ray Display::worldRayAtDisplayPosition(glm::vec2 pixel)
{
    ViewPoint *cam = viewpoints().front();
//    glm::vec3 cameraCenterOfFocus(cam->worldTransform() * cam->centerOfFocus());
//    glm::vec3 pixelPos = worldPositionAtDisplayPosition(pixel);
//    Geometry::printVector(pixelPos);
//    std::cout << std::endl;

//    Geometry::Ray ray(cameraCenterOfFocus, glm::normalize(pixelPos - cameraCenterOfFocus));
//    //ray.print();
//    return ray;
    return cam->worldRayAtDisplayPosition(pixel.x, pixel.y);
}

glm::vec3 Display::worldPositionAtDisplayPosition(glm::vec2 pixel)
{
//    Geometry::printVector(glm::vec3(pixel, 0));
//    Geometry::printVector(glm::vec3(resolution(), 0));
//    Geometry::printVector(glm::vec3(size(), 0));


    return glm::vec3(worldTransform() * glm::vec4(((pixel / glm::vec2(size())) - glm::vec2(0.5)) * dimensions(), 0 , 1));
}

glm::ivec2 Display::size()
{
    return m_glContext->defaultFramebufferSize();
}




OpenGLContext *Display::glContext() const
{
    return m_glContext;
}

void Display::setGlContext(OpenGLContext *glContext)
{
    m_glContext = glContext;
}


GLuint Display::scratchFrameBuffer() const
{
    return m_scratchFrameBuffer;
}
GLuint Display::scratchColorBufferTexture() const
{
    return m_scratchColorBufferTexture;
}
GLuint Display::scratchDepthBufferTexture() const
{
    return m_scratchDepthBufferTexture;
}

unsigned int next_pow2(unsigned int x)
{
    x -= 1;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

/* creates (and/or resizes) a framebuffer object to be used to draw to this display 
 * code adpted from https://codelab.wordpress.com/2014/09/07/oculusvr-sdk-and-simple-oculus-rift-dk2-opengl-test-program/
 */
void Display::createOrUpdateFBO(const char *fboName, uint &fbo, uint &fboColorBuffer, bool useColorTexture, uint &fboDepthBuffer, bool useDepthTexture, glm::ivec2 resolution)
{

    bool create = !fbo; //if fbo is null create all objects
    if(create){
        glGenFramebuffers(1, &fbo);
        
        if(useColorTexture){
            glGenTextures(1, &fboColorBuffer);
            glBindTexture(GL_TEXTURE_2D, fboColorBuffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }else{
            glGenRenderbuffers(1, &fboColorBuffer);
        }
        
        if(useDepthTexture){
            glGenTextures(1, &fboDepthBuffer);
            glBindTexture(GL_TEXTURE_2D, fboDepthBuffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }else{
            glGenRenderbuffers(1, &fboDepthBuffer);
        }
        
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    /* calculate the next power of two in both dimensions and use that as a texture size */
    glm::ivec2 fboTexRes = glm::ivec2(next_pow2(resolution.x), next_pow2(resolution.y));
    fboTexRes=resolution;

    if(useColorTexture){
        glBindTexture(GL_TEXTURE_2D, fboColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboTexRes.x, fboTexRes.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboColorBuffer, 0);
    }else{
        glBindRenderbuffer(GL_RENDERBUFFER, fboColorBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, fboTexRes.x, fboTexRes.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, fboColorBuffer);
    }
    if(useDepthTexture){
        glBindTexture(GL_TEXTURE_2D, fboDepthBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, fboTexRes.x, fboTexRes.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, fboDepthBuffer, 0);
    }else{
        glBindRenderbuffer(GL_RENDERBUFFER, fboDepthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fboTexRes.x, fboTexRes.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboDepthBuffer);
    }
   
    std::cout << "Checking Framebuffer \"" << std::string(fboName) << "\": ";
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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    printf("Successfully %s framebuffer \"%s\": %dx%d (texture size: %dx%d)\n", create ? "created" : "resized",
             fboName, resolution.x, resolution.y, fboTexRes.x, fboTexRes.y);
}







