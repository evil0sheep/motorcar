#ifndef DEFAULTDISPLAYNODE_H
#define DEFAULTDISPLAYNODE_H

#include "display.h"
#include "../wayland/waylandsurfacenode.h"
#include "../../../gl/openglcontext.h"
#include "../../../gl/openglshader.h"
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>



namespace motorcar {
class DefaultDisplayNode : public motorcar::Display
{
public:
    DefaultDisplayNode(OpenGLContext *glContext, PhysicalNode &parent, const glm::mat4 &transform);




    //inherited from Display
    void prepare() override;
    void renderSurfaceNode(motorcar::WaylandSurfaceNode *surfaceNode, motorcar::GLCamera *camera) override;
    motorcar::Geometry::Ray worldRayAtDisplayPosition(float pixelX, float pixelY) override;
    glm::ivec2 resolution() override;







    OpenGLContext *glContext() const;
    void setGlContext(motorcar::OpenGLContext *glContext);

private:

    motorcar::OpenGLContext *m_glContext;

    //shaders
    std::ifstream m_vertexShaderStream, m_fragmentShaderStream;
    motorcar::OpenGLShader *m_shaderProgram;

    //attribute buffers
    GLuint m_textureCoordinates, m_vertexCoordinates;

    //shader variable handles
    GLint h_aPosition, h_aTexCoord, h_uMVPMatrix;




};
}


#endif // DEFAULTDISPLAYNODE_H
