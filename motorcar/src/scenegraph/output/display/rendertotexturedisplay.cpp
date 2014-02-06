#include "rendertotexturedisplay.h"

using namespace motorcar;

RenderToTextureDisplay::RenderToTextureDisplay(float scale, glm::vec4 distortionK, OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode &parent, const glm::mat4 &transform)
    :Display(glContext, displayDimensions, parent, transform)
    ,m_scale(scale)
    ,m_distortionK(distortionK)
{
}

void RenderToTextureDisplay::renderSurfaceNode(WaylandSurfaceNode *surfaceNode, GLCamera *camera)
{
    Display::renderSurfaceNode(surfaceNode, camera);
}

glm::ivec2 RenderToTextureDisplay::resolution()
{
    return glm::ivec2(m_scale * glm::vec2(Display::resolution()));
}

glm::vec2 RenderToTextureDisplay::size() const
{
    return m_scale * Display::size();
}
