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
{
    m_segments = new float[numSegments * 2 * 3];
    memcpy (m_segments, segments, numSegments * 2 * 3 * sizeof (float)) ;
}

void WireframeNode::drawViewpoint(GLCamera *viewpoint)
{
    viewpoint->viewport()->display()->renderWireframeNode(this, viewpoint);
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

//void WireframeNode::setSegments(std::vector<std::pair<glm::vec3, glm::vec3> > &segments)
//{
//    int i =0;
//    delete m_segments;
//    m_segments = new float[segments.size() * 2 * 3];
//    m_numSegments = segments.size();

//    for(std::pair<glm::vec3, glm::vec3> p : segments){
//        segments[i][0][0] = p.first.x;
//        segments[i][0][1] = p.first.y;
//        segments[i][0][2] = p.first.z;
//        segments[i][1][0] = p.second.x;
//        segments[i][1][1] = p.second.y;
//        segments[i][1][2] = p.second.z;
//        i++;
//    }
//}
