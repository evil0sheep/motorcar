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
