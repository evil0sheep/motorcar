#ifndef RENDERTOTEXTUREDISPLAY_H
#define RENDERTOTEXTUREDISPLAY_H

#include "display.h"

namespace motorcar{
class RenderToTextureDisplay : public Display
{
public:
    RenderToTextureDisplay(float scale, glm::vec4 distortionK, OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode &parent, const glm::mat4 &transform = glm::mat4());

    //inherited from Display
    virtual void renderSurfaceNode(WaylandSurfaceNode *surfaceNode, GLCamera *camera) override;

    //inherited from Display
    virtual glm::ivec2 resolution() override;
    virtual glm::vec2 size() const override;
private:
    float m_scale;
    glm::vec4 m_distortionK;
};
}


#endif // RENDERTOTEXTUREDISPLAY_H
