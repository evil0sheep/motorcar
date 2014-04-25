#ifndef DEPTHCOMPOSITEDSURFACE_H
#define DEPTHCOMPOSITEDSURFACE_H
#include "waylandsurfacenode.h"


namespace motorcar {
class DepthCompositedSurfaceNode : public WaylandSurfaceNode
{
public:
    DepthCompositedSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4(1));



    ///inhereted from SceneGraphNode
    virtual Geometry::RaySurfaceIntersection *intersectWithSurfaces(const Geometry::Ray &ray) override;

    ///extracts the depth and color information from the client surface, clips them against the surface boundaries, and composites with the scene
    virtual void draw(Scene *scene, Display *display) override;






private:
    OpenGLShader *m_depthCompositedSurfaceShader, *m_depthCompositedSurfaceBlitter;
    void drawFrameBufferContents(Display * display);


    //attribute buffers
    GLuint m_colorTextureCoordinates, m_depthTextureCoordinates,  m_surfaceVertexCoordinates;

    //shader variable handles
    GLint h_aPosition, h_aColorTexCoord, h_aDepthTexCoord;

    GLint h_aPosition_blit, h_aTexCoord_blit, h_uColorSampler_blit, h_uDepthSampler_blit;

};
}


#endif // DEPTHCOMPOSITEDSURFACE_H
