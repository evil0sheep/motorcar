#ifndef WIREFRAMENODE_H
#define WIREFRAMENODE_H

#include "drawable.h"
#include <string.h>
#include "viewpoint.h"
#include "../../gl/openglshader.h"

namespace motorcar {
class WireframeNode : public Drawable
{
public:
   // WireframeNode(std::vector<std::pair<glm::vec3, glm::vec3> > &segments, glm::vec3 lineColor, SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4());

    WireframeNode(float *segments, int numSegments, glm::vec3 lineColor, SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4());

    virtual void draw(Scene *scene, Display *display) override;

    glm::vec3 lineColor() const;
    void setLineColor(const glm::vec3 &lineColor);

    int numSegments() const;
    float *segments() const;

private:
    float *m_segments;
    int m_numSegments;
    glm::vec3 m_lineColor;

    OpenGLShader *m_lineShader;

    GLuint m_lineVertexCoordinates;

    GLint h_aPosition_line, h_uMVPMatrix_line, h_uColor_line;
};
}


#endif // WIREFRAMENODE_H
