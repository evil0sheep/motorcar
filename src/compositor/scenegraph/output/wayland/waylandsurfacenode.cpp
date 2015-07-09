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
#include <scenegraph/output/wayland/waylandsurfacenode.h>
#include <scenegraph/output/display/display.h>
#include <gl/viewport.h>
#include <scenegraph/output/wireframenode.h>

using namespace motorcar;

WaylandSurfaceNode::WaylandSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform)
    :Drawable(parent, transform)
    ,m_surfaceShader(new motorcar::OpenGLShader(SHADER_MOTORCARSURFACE))

{

     std::cout << std::endl << "constructing surface node " << this << std::endl;

    static const GLfloat textureCoordinates[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };
    static const GLfloat vertexCoordinates[] ={
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };

    glGenBuffers(1, &m_surfaceTextureCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), textureCoordinates, GL_STATIC_DRAW);

    glGenBuffers(1, &m_surfaceVertexCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertexCoordinates, GL_STATIC_DRAW);



    h_aPosition_surface =  glGetAttribLocation(m_surfaceShader->handle(), "aPosition");
    h_aTexCoord_surface =  glGetAttribLocation(m_surfaceShader->handle(), "aTexCoord");
    h_uMVPMatrix_surface  = glGetUniformLocation(m_surfaceShader->handle(), "uMVPMatrix");

    if(h_aPosition_surface < 0 || h_aTexCoord_surface < 0 || h_uMVPMatrix_surface < 0 ){
       std::cout << "problem with surface shader handles: " << h_aPosition_surface << ", "<< h_aTexCoord_surface << ", " << h_uMVPMatrix_surface << std::endl;
    }

    std::vector<float> decorationVertices;
    //iterate over corners of box
    for(int i = -1; i <= 1; i += 2){
        for(int j = -1; j <= 1; j += 2){
            for(int k  = -1; k <= 1; k+=2){
                glm::vec3 cornerVertex = glm::vec3(i,j,k) * 0.5f;
                //iterate over corner segments
                for(int l = 0; l<3; l++){
                    decorationVertices.push_back(cornerVertex.x);
                    decorationVertices.push_back(cornerVertex.y);
                    decorationVertices.push_back(cornerVertex.z);
                    glm::vec3 secondVertex(cornerVertex);
                    glm::vec3 directions(i,j,k);
                    secondVertex[l] = secondVertex[l] - 0.25f * directions[l];
                    decorationVertices.push_back(secondVertex.x);
                    decorationVertices.push_back(secondVertex.y);
                    decorationVertices.push_back(secondVertex.z);
                }
            }
        }
    }

    glm::vec3 decorationColor(0.5f);

    m_decorationsNode = new WireframeNode(&(decorationVertices[0]), decorationVertices.size() / 6, decorationColor, this);
    this->setSurface(surface);

}

WaylandSurfaceNode::~WaylandSurfaceNode()
{
    std::cout << "deleting surfaceNode: " << this <<std::endl;
}

WaylandSurface *WaylandSurfaceNode::surface() const
{
    return m_surface;
}

void WaylandSurfaceNode::setSurface(WaylandSurface *surface)
{
    m_surface = surface;
    int type = this->surface()->type();
    if(type == WaylandSurface::SurfaceType::TOPLEVEL){
        m_decorationsNode->setVisible(true);
    }else{
        m_decorationsNode->setVisible(false);
    }
}

glm::mat4 WaylandSurfaceNode::surfaceTransform() const
{
    return m_surfaceTransform;
}

void WaylandSurfaceNode::computeSurfaceTransform(float ppcm)
{
    if(ppcm > 0){
        float ppm = ppcm * 100.f;
        glm::mat4 surfaceRotation = glm::rotate(glm::mat4(1), 180.f ,glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 surfaceScale = glm::scale(glm::mat4(1), glm::vec3( -m_surface->size().x / ppm,  m_surface->size().y / ppm, 1));
        glm::mat4 surfaceOffset = glm::translate(glm::mat4(1), glm::vec3(-0.5f, -0.5f, 0.0f));
        m_surfaceTransform = surfaceRotation * surfaceScale * surfaceOffset ;

        m_decorationsNode->setTransform(surfaceRotation*surfaceScale * glm::scale(glm::mat4(), glm::vec3(1.04, 1.04, 0.00)));
    }
}

bool WaylandSurfaceNode::computeLocalSurfaceIntersection(const Geometry::Ray &localRay, glm::vec2 &localIntersection, float &t)
{
    Geometry::Plane surfacePlane = Geometry::Plane(glm::vec3(0), glm::vec3(0.0f,0.0f,1.0f));
    if(glm::dot(localRay.d, surfacePlane.n) == 0) return false;

    Geometry::Ray transformedRay = localRay.transform(glm::inverse(surfaceTransform()));

    //transformedRay.print();

    t = surfacePlane.intersect(transformedRay);
    //std::cout << "t = " << t << std::endl;
    glm::vec3 intersection = transformedRay.solve(t);

    //Geometry::printVector(glm::vec3(intersection));

    //transformedRay.print();
    //transformedRay.draw(this, glm::vec3(0,0,1),  surfaceTransform());

    glm::vec3 coords= intersection * glm::vec3(m_surface->size().x, m_surface->size().y, 0);
    localIntersection =  glm::vec2(coords);

    return t >= 0;
}

Geometry::RaySurfaceIntersection *WaylandSurfaceNode::intersectWithSurfaces(const Geometry::Ray &ray)
{
    Geometry::RaySurfaceIntersection *closestSubtreeIntersection = SceneGraphNode::intersectWithSurfaces(ray);

    if(((int) m_surface->type()) == WaylandSurface::SurfaceType::CURSOR){
        return closestSubtreeIntersection;
    }

    Geometry::Ray localRay = ray.transform(inverseTransform());

    float t;
    glm::vec2 localIntersection;
    bool isIntersected = computeLocalSurfaceIntersection(localRay, localIntersection, t);


    if(isIntersected && (closestSubtreeIntersection == NULL || t < closestSubtreeIntersection-> t)
            && localIntersection.x >= 0 && localIntersection.x <= m_surface->size().x
            && localIntersection.y >= 0 && localIntersection.y <= m_surface->size().y){

            return new Geometry::RaySurfaceIntersection(this, localIntersection, ray, t);


    }else{
        return closestSubtreeIntersection;
    }
}

void WaylandSurfaceNode::draw(Scene *scene, Display *display)
{
    //std::cout << "drawing surface node " << this <<std::endl;

    GLuint texture = this->surface()->texture();

    glUseProgram(m_surfaceShader->handle());

    glEnableVertexAttribArray(h_aPosition_surface);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glVertexAttribPointer(h_aPosition_surface, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(h_aTexCoord_surface);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
    glVertexAttribPointer(h_aTexCoord_surface, 2, GL_FLOAT, GL_FALSE, 0, 0);



    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    for(ViewPoint *viewpoint : display->viewpoints()){
        viewpoint->viewport()->set();
        glUniformMatrix4fv(h_uMVPMatrix_surface, 1, GL_FALSE, glm::value_ptr(viewpoint->projectionMatrix() * viewpoint->viewMatrix() *  this->worldTransform() * this->surfaceTransform()));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    }
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(h_aPosition_surface);
    glDisableVertexAttribArray(h_aTexCoord_surface);

    glUseProgram(0);
}

void WaylandSurfaceNode::handleFrameBegin(Scene *scene)
{
    Drawable::handleFrameBegin(scene);
    if(visible()){
        computeSurfaceTransform(8);
        surface()->prepare();

    }

}


bool WaylandSurfaceNode::mapped() const
{
    return m_mapped;
}

void WaylandSurfaceNode::setMapped(bool mapped)
{
    m_mapped = mapped;
}
bool WaylandSurfaceNode::damaged() const
{
    return m_damaged;
}

void WaylandSurfaceNode::setDamaged(bool damaged)
{
    m_damaged = damaged;
}


