#ifndef WAYLANDSURFACENODE_H
#define WAYLANDSURFACENODE_H
#include "../drawable.h"
#include "../../../gl/openglshader.h"
#include "../../../gl/openglcontext.h"
#include "../../../wayland/output/waylandsurface.h"


namespace motorcar {
class WireframeNode;
class WaylandSurfaceNode : public Drawable
{
public:


    WaylandSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4(1));
    virtual ~WaylandSurfaceNode();

    ///computes surface transform
    virtual void computeSurfaceTransform(float ppcm);

    ///inhereted from SceneGraphNode
    virtual Geometry::RaySurfaceIntersection *intersectWithSurfaces(const Geometry::Ray &ray) override;

    ///inhereted from Drawable
    virtual void draw(Scene *scene, Display *display) override;

    ///prepares the surface and computes the surface transform
    virtual void handleFrameBegin(Scene *scene) override;



    WaylandSurface *surface() const;
    void setSurface(WaylandSurface *surface);
    //returns the transform which maps normalized surface coordinates to the local node space
    glm::mat4 surfaceTransform() const;


    bool isSurfaceNode() override {return true;}


    bool mapped() const;
    void setMapped(bool mapped);

    bool damaged() const;
    void setDamaged(bool damaged);

private:

    //takes a ray in the local Node space and returns whether or not the ray insersects the plane of this surface;
    // t: the ray's intersection distance to the surface
    // localIntersection : the ray's intersection with the surface in wayland "surface local coordinates" as a QPoint for use with QTWayland
    virtual bool computeLocalSurfaceIntersection(const Geometry::Ray &localRay, glm::vec2 &localIntersection,  float &t);


    WaylandSurface *m_surface;


    bool m_mapped;
    bool m_damaged;


    //attribute buffers
    GLuint m_surfaceTextureCoordinates, m_surfaceVertexCoordinates;

protected:


    //shader variable handles
    GLint h_aPosition_surface, h_aTexCoord_surface, h_uMVPMatrix_surface;

    OpenGLShader *m_surfaceShader;

    glm::mat4 m_surfaceTransform;
    WireframeNode *m_decorationsNode;





};
}


#endif // WAYLANDSURFACENODE_H
