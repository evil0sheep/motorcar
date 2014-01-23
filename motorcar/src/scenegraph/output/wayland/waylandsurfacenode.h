#ifndef WAYLANDSURFACENODE_H
#define WAYLANDSURFACENODE_H
#include "../drawable.h"

namespace motorcar {
class WaylandSurfaceNode : public Drawable
{
public:
    WaylandSurfaceNode();

    struct RaySurfaceIntersection
    {
        RaySurfaceIntersection(WaylandSurfaceNode *surfaceNode, glm::vec2 surfaceLocalCoordinates , const Geometry::Ray &ray , float t );
        WaylandSurfaceNode *surfaceNode;
        glm::vec2 surfaceLocalCoordinates;
        glm::vec3 nodeLocalCoordinates;
        Geometry::Ray ray;
        float t;
    };
};
}


#endif // WAYLANDSURFACENODE_H
