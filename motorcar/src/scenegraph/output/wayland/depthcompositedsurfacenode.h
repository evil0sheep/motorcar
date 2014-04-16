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

    ///extracts the depth and color information from the client surface, clips them against the surface boundaries, and composites with the scene
    virtual void draw(Scene *scene, Display *display) override;

    ///prepares the surface and computes the surface transform
    virtual void handleFrameBegin(Scene *scene) override;

private:
    //attribute buffers
    GLuint m_surfaceTextureCoordinates, m_surfaceVertexCoordinates;
};
}


#endif // DEPTHCOMPOSITEDSURFACE_H
