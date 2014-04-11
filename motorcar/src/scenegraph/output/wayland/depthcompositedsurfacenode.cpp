#include "depthcompositedsurfacenode.h"
#include "../display/display.h"

using namespace motorcar;
DepthCompositedSurfaceNode::DepthCompositedSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform)
    :WaylandSurfaceNode(surface, parent, transform)
{
}

void DepthCompositedSurfaceNode::computeSurfaceTransform(float ppcm)
{
    m_surfaceTransform = glm::translate(glm::mat4(), glm::vec3(-1, -1, 0)) * glm::scale(glm::mat4(), glm::vec3(2, 2, 1));
}

Geometry::RaySurfaceIntersection *DepthCompositedSurfaceNode::intersectWithSurfaces(const Geometry::Ray &ray)
{
    return SceneGraphNode::intersectWithSurfaces(ray);
}

void DepthCompositedSurfaceNode::drawViewpoint(ViewPoint *viewpoint)
{
    computeSurfaceTransform(8);
    surface()->prepare();
    viewpoint->viewport()->display()->renderDepthCompositedSurfaceNode(this, viewpoint);
}
