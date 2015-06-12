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
#include <scenegraph/output/wireframenode.h>
#include <scenegraph/output/display/display.h>

#include <string>

using namespace motorcar;

//WireframeNode::WireframeNode(std::vector<std::pair<glm::vec3, glm::vec3>> &segments, glm::vec3 lineColor, SceneGraphNode *parent, const glm::mat4 &transform)
//    :Drawable(parent, transform)
//    ,m_segments(NULL)
//    ,m_lineColor(lineColor)
//{
//    setSegments(segments);
//}

WireframeNode::WireframeNode(float *segments, int numSegments, glm::vec3 lineColor, SceneGraphNode *parent, const glm::mat4 &transform)
    :Drawable(parent, transform)
    ,m_segments(NULL)
    ,m_numSegments(numSegments)
    ,m_lineColor(lineColor)
    ,m_lineShader(new motorcar::OpenGLShader(SHADER_MOTORCARLINE))
{
    m_segments = new float[numSegments * 2 * 3];
    memcpy (m_segments, segments, numSegments * 2 * 3 * sizeof (float)) ;

    glGenBuffers(1, &m_lineVertexCoordinates);
    h_aPosition_line =  glGetAttribLocation(m_lineShader->handle(), "aPosition");
    h_uColor_line =  glGetUniformLocation(m_lineShader->handle(), "uColor");
    h_uMVPMatrix_line  = glGetUniformLocation(m_lineShader->handle(), "uMVPMatrix");

    if(h_aPosition_line < 0 || h_uColor_line < 0 || h_uMVPMatrix_line < 0 ){
       std::cout << "problem with line shader handles: " << h_aPosition_line << ", "<< h_uColor_line << ", " << h_uMVPMatrix_line << std::endl;
    }

}

void WireframeNode::draw(Scene *scene, Display *display)
{
    glUseProgram(m_lineShader->handle());

    glEnableVertexAttribArray(h_aPosition_line);
    glBindBuffer(GL_ARRAY_BUFFER, m_lineVertexCoordinates);
    glVertexAttribPointer(h_aPosition_line, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBufferData(GL_ARRAY_BUFFER,  this->numSegments() * 2 * 3 * sizeof(float), this->segments(), GL_DYNAMIC_DRAW);

    glUniform3fv(h_uColor_line, 1, glm::value_ptr(this->lineColor()));


    for(ViewPoint *viewpoint : display->viewpoints()){
        //Geometry::printMatrix(viewpoint->viewMatrix());
        glUniformMatrix4fv(h_uMVPMatrix_line, 1, GL_FALSE, glm::value_ptr(viewpoint->projectionMatrix() * viewpoint->viewMatrix() *  this->worldTransform()));
        viewpoint->viewport()->set();
        glDrawArrays(GL_LINES, 0, 2 * this->numSegments());

    }
    glDisableVertexAttribArray(h_aPosition_line);

    glUseProgram(0);
}



glm::vec3 WireframeNode::lineColor() const
{
    return m_lineColor;
}

void WireframeNode::setLineColor(const glm::vec3 &lineColor)
{
    m_lineColor = lineColor;
}

int WireframeNode::numSegments() const
{
    return m_numSegments;
}


float *WireframeNode::segments() const
{
    return m_segments;
}

