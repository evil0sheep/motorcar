#ifndef DISPLAY_H
#define DISPLAY_H
#include "../outputelement.h"
#include "../glcameranode.h"
#include "../drawable.h"
#include "../wireframenode.h"
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
    Display(OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~Display();

    //set up  and tear down for drawing, make all calls that need to be made only once per frame
    virtual void prepareForDraw();
    virtual void finishDraw() {}

    //renders the given drawable from all viewpoints
    void renderDrawable(Drawable *node);

    //renders the given surface from the given viewpoint in an implementation specific manner
    virtual void renderSurfaceNode(WaylandSurfaceNode *surfaceNode, ViewPoint *camera);

    virtual void renderWireframeNode(WireframeNode *node, ViewPoint *camera);

    //for legacy mouse support
    //projects mouse position into worldpace based on implementation specific details
    virtual Geometry::Ray worldRayAtDisplayPosition(glm::vec2 pixel);

    glm::vec3 worldPositionAtDisplayPosition(glm::vec2 pixel);

    //returns resolution of this display in pixels
    virtual glm::ivec2 resolution();

    //returns the size of this display in meters
    virtual glm::vec2 size() const;


    void addViewpoint(ViewPoint *v);

    std::vector<ViewPoint *> viewpoints() const;




    OpenGLContext *glContext() const;
    void setGlContext(OpenGLContext *glContext);





private:
    std::vector<ViewPoint *> m_viewpoints;
    glm::vec2 m_size;
    OpenGLContext *m_glContext;

    //shaders
    motorcar::OpenGLShader *m_surfaceShader;
    motorcar::OpenGLShader *m_lineShader;

    //attribute buffers
    GLuint m_surfaceTextureCoordinates, m_surfaceVertexCoordinates;
    GLuint m_lineVertexCoordinates;

    //shader variable handles
    GLint h_aPosition_surface, h_aTexCoord_surface, h_uMVPMatrix_surface;
    GLint h_aPosition_line, h_uMVPMatrix_line, h_uColor_line;

};
}



#endif // DISPLAY_H
