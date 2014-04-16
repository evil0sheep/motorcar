#include "wireframenode.h"
#include "display/display.h"

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
    ,m_lineShader(new motorcar::OpenGLShader(std::string("../motorcar/src/shaders/motorcarline.vert"), std::string("../motorcar/src/shaders/motorcarline.frag")))

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

