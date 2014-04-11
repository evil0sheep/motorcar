#ifndef DEPTHCOMPOSITEDSURFACE_H
#define DEPTHCOMPOSITEDSURFACE_H
#include "waylandsurfacenode.h"

namespace motorcar {
class DepthCompositedSurfaceNode : public WaylandSurfaceNode
{
public:
    DepthCompositedSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4(1));

    ///computes surface transform
    virtual void computeSurfaceTransform(float ppcm) override;

    ///inhereted from SceneGraphNode
    virtual Geometry::RaySurfaceIntersection *intersectWithSurfaces(const Geometry::Ray &ray) override;

    ///inhereted from Drawable
    virtual void drawViewpoint(ViewPoint *viewpoint) override;
};
}


#endif // DEPTHCOMPOSITEDSURFACE_H
