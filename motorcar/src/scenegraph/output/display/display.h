#ifndef DISPLAY_H
#define DISPLAY_H
#include "../outputelement.h"
#include "../glcameranode.h"
#include "../drawable.h"
#include "../../physicalnode.h"
#include "../../../gl/openglshader.h"
#include "../../../gl/openglcontext.h"
#include "../wayland/waylandsurfacenode.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


namespace motorcar {
class WaylandSurface;
class Display : public OutputElement, public PhysicalNode
{
public:
    Display(OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode &parent, const glm::mat4 &transform = glm::mat4());
    virtual ~Display();

    virtual void prepare();

    //renders the given drawable from all viewpoints
    void renderDrawable(Drawable *node);

    //renders the given surface from the given viewpoint in an implementation specific manner
    virtual void renderSurfaceNode(WaylandSurfaceNode *surfaceNode, GLCamera *camera);

    //for legacy mouse support
    //projects mouse position into worldpace based on implementation specific details
    virtual Geometry::Ray worldRayAtDisplayPosition(glm::vec2 pixel);

    glm::vec3 worldPositionAtDisplayPosition(glm::vec2 pixel);

    //returns resolution of this display in pixels
    virtual glm::ivec2 resolution();

    //returns the size of this display in meters
    virtual glm::vec2 size() const;


    void addViewpoint(GLCamera *v);

    std::vector<GLCamera *> viewpoints() const;




    OpenGLContext *glContext() const;
    void setGlContext(OpenGLContext *glContext);

private:
    std::vector<GLCamera *> m_viewpoints;
    glm::vec2 m_size;
    OpenGLContext *m_glContext;

    //shaders
    std::ifstream m_vertexShaderStream, m_fragmentShaderStream;
    motorcar::OpenGLShader *m_shaderProgram;

    //attribute buffers
    GLuint m_textureCoordinates, m_vertexCoordinates;

    //shader variable handles
    GLint h_aPosition, h_aTexCoord, h_uMVPMatrix;

};
}



#endif // DISPLAY_H
