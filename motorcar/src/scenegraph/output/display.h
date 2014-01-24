#ifndef DISPLAY_H
#define DISPLAY_H
#include "outputelement.h"
#include "wayland/waylandsurfacenode.h"
#include <glm/glm.hpp>
#include <vector>


namespace motorcar {
class Display : public OutputElement
{
public:
    Display(std::vector<GLCamera *> viewpoints);
    virtual ~Display();

    //renders the given drawable from all viewpoints
    void renderDrawable(Drawable *node);

    //renders the given surface from the given viewpoint in an implementation specific manner
    virtual void renderSurfaceNode(WaylandSurfaceNode *surface, GLCamera *viewpoint) = 0;

    //for legacy mouse support
    //projects mouse position into worldpace based on implementation specific details
    virtual Geometry::Ray worldRayAtDisplayPosition(float pixelX, float pixelY) = 0;

    //returns size of this display in pixels
    virtual glm::vec2 size() = 0;

private:
     std::vector<GLCamera *> m_viewpoints;

};
}



#endif // DISPLAY_H
