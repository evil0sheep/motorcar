#ifndef DISPLAY_H
#define DISPLAY_H
#include "../viewpoint.h"
#include "../../physicalnode.h"
#include "../../../gl/openglcontext.h"
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


namespace motorcar {
class WaylandSurface;
class Display : public PhysicalNode
{
public:
    Display(OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~Display();

    //set up  and tear down for drawing, make all calls that need to be made only once per frame
    virtual void prepareForDraw();
    virtual void finishDraw() {}


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





};
}



#endif // DISPLAY_H
