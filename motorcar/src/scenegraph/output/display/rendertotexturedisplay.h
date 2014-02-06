#ifndef RENDERTOTEXTUREDISPLAY_H
#define RENDERTOTEXTUREDISPLAY_H

#include "display.h"

namespace motorcar{
class RenderToTextureDisplay : public Display
{
public:
    RenderToTextureDisplay(float scale, glm::vec4 distortionK, OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode &parent, const glm::mat4 &transform = glm::mat4());
    ~RenderToTextureDisplay();

    //inherited from Display
    virtual void prepareForDraw() override;
    virtual void finishDraw() override;
    virtual void renderSurfaceNode(WaylandSurfaceNode *surfaceNode, GLCamera *camera) override;

    //inherited from Display, apply scaling factor to base class output
    virtual glm::ivec2 resolution() override;
    virtual glm::vec2 size() const override;
private:
    float m_scale;
    glm::vec4 m_distortionK;
    GLuint m_frameBuffer, m_frameBufferTexture, m_depthBuffer, m_textureCoordinates_distortion, m_vertexCoordinates_distortion;
    //shaders
    motorcar::OpenGLShader *m_distortionShader;

    //shader variable handles
    GLint h_aPosition_distortion, h_aTexCoord_distortion;

};
}


#endif // RENDERTOTEXTUREDISPLAY_H
