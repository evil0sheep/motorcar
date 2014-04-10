#ifndef WIREFRAMENODE_H
#define WIREFRAMENODE_H

#include "drawable.h"
#include <string.h>


namespace motorcar {
class WireframeNode : public Drawable
{
public:
   // WireframeNode(std::vector<std::pair<glm::vec3, glm::vec3> > &segments, glm::vec3 lineColor, SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4());

    WireframeNode(float *segments, int numSegments, glm::vec3 lineColor, SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4());

    //inhereted from Drawable
    void drawViewpoint(ViewPoint *viewpoint) override;

    glm::vec3 lineColor() const;
    void setLineColor(const glm::vec3 &lineColor);

    int numSegments() const;


    float *segments() const;
    //void setSegments(std::vector<std::pair<glm::vec3, glm::vec3> > &segments);

private:
    float *m_segments;
    int m_numSegments;
    glm::vec3 m_lineColor;
};
}


#endif // WIREFRAMENODE_H
