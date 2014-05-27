#ifndef DEPTHCOMPOSITEDSURFACE_H
#define DEPTHCOMPOSITEDSURFACE_H
#include "waylandsurfacenode.h"


namespace motorcar {

class DepthCompositedSurfaceNode : public WaylandSurfaceNode
{
public:
    DepthCompositedSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4(1), glm::vec3 dimensions = glm::vec3(1));





    ///extracts the depth and color information from the client surface, clips them against the surface boundaries, and composites with the scene
    virtual void draw(Scene *scene, Display *display) override;
    virtual void computeSurfaceTransform(float ppcm) override;

    void handleWorldTransformChange(Scene *scene) override;


    //returns the dimensions of the 3D window associated with this surface node
    glm::vec3 dimensions() const;
    //request the client resize the 3D window to the specified size, in meters
    //no change will be made until the client responds with a set_size_3D request
    void requestSize3D(const glm::vec3 &dimensions);

    static void handle_set_size_3d(struct wl_client *client,
                    struct wl_resource *resource,
                    struct wl_array *dimensions);

    wl_resource *resource() const;
    void configureResource(struct wl_client *client, uint32_t id);

private:
    bool computeLocalSurfaceIntersection(const Geometry::Ray &localRay, glm::vec2 &localIntersection,  float &t) override;

    void sendTransformToClient();
    void setDimensions(const glm::vec3 &dimensions);

    OpenGLShader *m_depthCompositedSurfaceShader, *m_depthCompositedSurfaceBlitter, *m_clippingShader;
    void drawFrameBufferContents(Display * display);
    void drawWindowBoundsStencil(Display * display);

    //attribute buffers
    GLuint m_colorTextureCoordinates, m_depthTextureCoordinates,  m_surfaceVertexCoordinates;
    GLuint m_cuboidClippingVertices, m_cuboidClippingIndices;

    //shader variable handles
    GLint h_aPosition, h_aColorTexCoord, h_aDepthTexCoord;

    GLint h_aPosition_blit, h_aTexCoord_blit, h_uColorSampler_blit, h_uDepthSampler_blit;

    GLint h_aPosition_clipping, h_uMVPMatrix_clipping, h_uColor_clipping;


    struct wl_resource *m_resource;
    struct wl_array m_dimensionsArray, m_transformArray;
    glm::vec3 m_dimensions;

};
}


#endif // DEPTHCOMPOSITEDSURFACE_H
