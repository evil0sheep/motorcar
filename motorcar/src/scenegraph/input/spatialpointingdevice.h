#ifndef SPATIALMOUSE_H
#define SPATIALMOUSE_H

#include <glm/gtc/type_ptr.hpp>

#include "inputelement.h"
#include "../physicalnode.h"
#include "../output/wireframenode.h"
#include "../output/wayland/waylandsurfacenode.h"

namespace motorcar {
class SpatialPointingDevice : public InputElement, public PhysicalNode
{
public:
    SpatialPointingDevice(PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~SpatialPointingDevice() {}

    //inhereted from SceneGraphNode
    void traverseNode(Scene *scene, long deltaMillis) override;



    bool leftMouseDown() const;
    void setLeftMouseDown(bool leftMouseDown);

    bool rightMouseDown() const;
    void setRightMouseDown(bool rightMouseDown);

    bool middleMouseDown() const;
    void setMiddleMouseDown(bool middleMouseDown);



    void grabSurfaceUnderCursor();
    void releaseGrabbedSurface();



private:

    void mouseEvent(WaylandSurface::MouseEvent eventType, WaylandSurface::MouseButton button);

    Geometry::RaySurfaceIntersection *m_latestIntersection;
    bool m_leftMouseDown, m_rightMouseDown, m_middleMouseDown;

    WaylandSurfaceNode *m_grabbedSurfaceNode;
    glm::mat4 m_grabbedSurfaceNodeTransform;



};
}


#endif // SPATIALMOUSE_H
