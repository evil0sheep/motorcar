/****************************************************************************
**This file is part of the Motorcar 3D windowing framework
**
**
**Copyright (C) 2014 Forrest Reiling
**
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/
#ifndef DEPTHCOMPOSITEDSURFACE_H
#define DEPTHCOMPOSITEDSURFACE_H
#include <scenegraph/output/wayland/waylandsurfacenode.h>


namespace motorcar {

class MotorcarSurfaceNode : public WaylandSurfaceNode
{
public:
    MotorcarSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4(1), glm::vec3 dimensions = glm::vec3(1));





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
    void clipWindowBounds(Display * display);

    //attribute buffers
    GLuint m_colorTextureCoordinates, m_depthTextureCoordinates,  m_surfaceVertexCoordinates;
    GLuint m_cuboidClippingVertices, m_cuboidClippingIndices;

    //shader variable handles
    GLint h_aPosition_depthcomposite, h_aColorTexCoord_depthcomposite, h_aDepthTexCoord_depthcomposite;

    GLint h_aPosition_blit, h_aTexCoord_blit, h_uColorSampler_blit, h_uDepthSampler_blit;

    GLint h_aPosition_clipping, h_uMVPMatrix_clipping, h_uColor_clipping;


    struct wl_resource *m_resource;
    struct wl_array m_dimensionsArray, m_transformArray;


    glm::vec3 m_dimensions;

};
}


#endif // DEPTHCOMPOSITEDSURFACE_H
