#ifndef DISPLAY_H
#define DISPLAY_H
#include "outputelement.h"
#include "glcameranode.h"
#include "drawable.h"
#include <glm/glm.hpp>
#include <vector>


namespace motorcar {
class WaylandSurface;
class Display : public OutputElement
{
public:
    Display();
    Display(std::vector<GLCamera *> viewpoints);
    virtual ~Display();

    virtual void prepare();

    //renders the given drawable from all viewpoints
    void renderDrawable(Drawable *node);

    //renders the given surface from the given viewpoint in an implementation specific manner
    virtual void renderSurfaceNode(WaylandSurfaceNode *surfaceNode, GLCamera *camera) = 0;

    //for legacy mouse support
    //projects mouse position into worldpace based on implementation specific details
    virtual Geometry::Ray worldRayAtDisplayPosition(float pixelX, float pixelY) = 0;

    //returns size of this display in pixels
    virtual glm::ivec2 size() = 0;

    void addViewpoint(GLCamera *v);

    std::vector<GLCamera *> viewpoints() const;


private:
    std::vector<GLCamera *> m_viewpoints;

};
}



#endif // DISPLAY_H
