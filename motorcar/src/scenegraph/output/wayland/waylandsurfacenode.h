#ifndef WAYLANDSURFACENODE_H
#define WAYLANDSURFACENODE_H
#include "waylanddrawable.h"
#include "waylandsurface.h"


namespace motorcar {
class WaylandSurfaceNode : public WaylandDrawable
{
public:


    WaylandSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4(1));
    virtual ~WaylandSurfaceNode();

    WaylandSurface *surface() const;
    void setSurface(WaylandSurface *surface);
    //returns the transform which maps normalized surface coordinates to the local node space
    glm::mat4 surfaceTransform() const;
    //computes surface transform
    void computeSurfaceTransform(float ppcm);


    //takes a ray in the local Node space and returns whether or not the ray insersects the plane of this surface;
    // t: the ray's intersection distance to the surface
    // localIntersection : the ray's intersection with the surface in wayland "surface local coordinates" as a QPoint for use with QTWayland
    bool computeLocalSurfaceIntersection(const Geometry::Ray &localRay, glm::vec2 &localIntersection,  float &t);


    //inhereted from SceneGraphNode
    virtual Geometry::RaySurfaceIntersection *intersectWithSurfaces(const Geometry::Ray &ray) override;

    //inhereted from Drawable
    void drawViewpoint(GLCamera *viewpoint) override;

    bool mapped() const;
    void setMapped(bool mapped);

    bool damaged() const;
    void setDamaged(bool damaged);

private:
    WaylandSurface *m_surface;
    glm::mat4 m_surfaceTransform;

    bool m_mapped;
    bool m_damaged;




};
}


#endif // WAYLANDSURFACENODE_H
