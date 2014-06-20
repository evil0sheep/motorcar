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
#include <gl/viewport.h>

using namespace motorcar;

ViewPort::ViewPort(glm::vec2 normalizedPos, glm::vec2 normalizedSize, Geometry::Rectangle *bufferGeometry)
    : m_offsetX(normalizedPos.x)
    , m_offsetY(normalizedPos.y)
    , m_width(normalizedSize.x)
    , m_height(normalizedSize.y)
    , m_bufferGeometry(bufferGeometry)
{

}

void ViewPort::set() const
{
    glViewport(offsetX(), offsetY(), width(), height());
}

glm::vec2 ViewPort::displayCoordsToViewportCoords(float pixelX, float pixelY) const
{
    return glm::vec2(((pixelX - offsetX()) / width() - 0.5f), ((pixelY - offsetY()) / height()  - 0.5f) * (height() / width()));
}

void ViewPort::uvCoords(float *buf)
{
    buf[0] = m_offsetX;
    buf[1] = m_offsetY;

    buf[2] = m_offsetX;
    buf[3] = m_offsetY + m_height;

    buf[4] = m_offsetX + m_width;
    buf[5] = m_offsetY + m_height;

    buf[6] = m_offsetX + m_width;
    buf[7] = m_offsetY;


}


float ViewPort::height() const
{
    return m_height * m_bufferGeometry->size().y;
}



float ViewPort::width() const
{
    return m_width * m_bufferGeometry->size().x;
}

float ViewPort::offsetY() const
{
    return m_offsetY * m_bufferGeometry->size().y;
}





float ViewPort::offsetX() const
{
    return m_offsetX * m_bufferGeometry->size().x;
}


glm::ivec2 ViewPort::size()
{
    return glm::ivec2(width(), height());
}
