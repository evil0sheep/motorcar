#ifndef RENDERTOTEXTUREDISPLAY_H
#define RENDERTOTEXTUREDISPLAY_H

#include "display.h"

namespace motorcar{
class RenderToTextureDisplay : public Display
{
public:
    RenderToTextureDisplay(float scale, glm::vec4 distortionK, OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~RenderToTextureDisplay();

    //inherited from Display
    virtual void prepareForDraw() override;
    virtual void finishDraw() override;
    virtual void renderSurfaceNode(WaylandSurfaceNode *surfaceNode, ViewPoint *camera) override;
    virtual void renderWireframeNode(WireframeNode *node, ViewPoint *camera) override;

    //inherited from Display, apply scaling factor to base class output
    virtual glm::ivec2 resolution() override;
    virtual glm::vec2 size() const override;
private:
    float m_scale;
    glm::vec4 m_distortionK;
    GLuint m_frameBuffer, m_frameBufferTexture, m_depthBuffer, m_surfaceTextureCoordinates, m_surfaceVertexCoordinates;
    //shaders
    motorcar::OpenGLShader *m_distortionShader;

    //shader variable handles
    GLint h_aPosition_distortion, h_aTexCoord_distortion, h_uDistortionK, h_uLenseCenter, h_uViewportParams, h_uScaleFactor;


};
}


#endif // RENDERTOTEXTUREDISPLAY_H
