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
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <geometry.h>
#include <GL/gl.h>
namespace motorcar{

//normalized viewport within a given window,
//all numerical arguments range from 0 to 1 and are multiplied by the size of the window in all getters and before being passed to OpenGL calls
class ViewPort : public Geometry::Rectangle
{
public:

    ViewPort(glm::vec2 normalizedPos, glm::vec2 normalizedSize, Rectangle *bufferGeometry);
    ~ViewPort() {}

    float offsetX() const;
    float offsetY() const;
    float width() const;
    float height() const;


    //returns resolution of this viewport in pixels, inherited from rectangle
    virtual glm::ivec2 size() override;

    //calls glViewport with parameters
    void set() const;


    glm::vec2 displayCoordsToViewportCoords(float pixelX, float pixelY) const;

    //takes a buffer of at least 8 floats and fills it with the UV space texture coordinates for the four corners of the viewport
    void uvCoords(float *buf);

    //returns a vector of the four floats which this was constructed on
    glm::vec4 viewportParams() const;



    Rectangle *bufferGeometry() const;
    void setBufferGeometry(Rectangle *bufferGeometry);

private:
    Rectangle *m_bufferGeometry;
    float m_offsetX, m_offsetY, m_width, m_height;

};
}



#endif // VIEWPORT_H
