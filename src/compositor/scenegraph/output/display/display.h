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
#ifndef DISPLAY_H
#define DISPLAY_H
#include <scenegraph/output/viewpoint.h>
#include <scenegraph/physicalnode.h>
#include <gl/openglcontext.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


namespace motorcar {
class WaylandSurface;
class Display : public PhysicalNode, public Geometry::Rectangle
{
public:
    Display(OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~Display();

    //set up  and tear down for drawing, make all calls that need to be made only once per frame
    virtual void prepareForDraw();
    virtual void finishDraw() {}


    //for legacy mouse support
    //projects mouse position into worldpace based on implementation specific details
    virtual Geometry::Ray worldRayAtDisplayPosition(glm::vec2 pixel);

    glm::vec3 worldPositionAtDisplayPosition(glm::vec2 pixel);

    //returns resolution of this display in pixels, inherited from rectangle
    virtual glm::ivec2 size() override;

    //returns the size of this display in meters
    virtual glm::vec2 dimensions() const;


    void addViewpoint(ViewPoint *v);

    std::vector<ViewPoint *> viewpoints() const;


    OpenGLContext *glContext() const;
    void setGlContext(OpenGLContext *glContext);

    virtual GLuint activeFrameBuffer() const {return 0;}
    virtual GLuint depthBufferTexture() const {return 0;}

    GLuint scratchFrameBuffer() const;
    GLuint scratchColorBufferTexture() const;
    GLuint scratchDepthBufferTexture() const;


private:
    std::vector<ViewPoint *> m_viewpoints;
    glm::vec2 m_dimensions;
    OpenGLContext *m_glContext;

protected:
    GLuint m_scratchFrameBuffer, m_scratchColorBufferTexture, m_scratchDepthBufferTexture;





};
}



#endif // DISPLAY_H
