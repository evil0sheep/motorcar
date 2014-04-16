/****************************************************************************
**This file is part of the MotorCar QtWayland Compositor
**
**
**Copyright (C) 2013 Forrest Reiling
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

#include "geometry.h"
#include "scenegraph/scene.h"
#include "scenegraph/output/wireframenode.h"
#include <glm/gtc/matrix_inverse.hpp>


using namespace motorcar;

Geometry::Ray::Ray(glm::vec3 p, glm::vec3 d)
    : p(p)
    , d(d)
{}

Geometry::Ray Geometry::Ray::transform(glm::mat4 t) const
{
    return Ray(glm::vec3(t * glm::vec4(p, 1)), glm::vec3(t * glm::vec4(d, 0)));
}

glm::vec3 Geometry::Ray::solve(float t) const
{
    return p + (t * d);
}

void Geometry::Ray::print() const
{
    std::cout << "p : <" << p.x << ", " << p.y << ", " << p.z <<  "> d : <" << d.x << ", " << d.y << ", " << d.z <<  ">" << std::endl;
}

void Geometry::Ray::draw(SceneGraphNode *parent, glm::vec3 color, glm::mat4 transform)
{
    glm::mat4 translation = glm::translate(glm::mat4(), p);
    float vertices[]= {
        -0.05, 0, 0,
        0.05, 0, 0,
        0, -0.05, 0,
        0, 0.05, 0,
        0, 0, -0.05,
        0, 0, 0.05,
        0, 0, 0,
        d.x, d.y, d.z

    };
    WireframeNode *node = new WireframeNode(vertices, 4, color, parent, transform*translation);

    Scene *scene = parent->scene();
    for(Display *display : scene->displays()){
        node->draw(scene, display);
    }

    delete node;
}




Geometry::Plane::Plane(glm::vec3 p, glm::vec3 n)
    : p(p)
    , n(n)
{}

float Geometry::Plane::intersect(Geometry::Ray r)
{
    return glm::dot(p - r.p,  n) / glm::dot(r.d, n);
}




//        glm::vec3 vec = pos;
//        qDebug() << "pos: <" << vec.x << ", " << vec.y << ", " << vec.z <<  ">";
void Geometry::printMatrix(glm::mat4 m)
{
    for(int i=0; i<4; i++){
            glm::vec4 row= glm::row(m, i);
            printf("%2.2f %2.2f %2.2f %2.2f\n", row.x, row.y, row.z, row.w);
        }
    printf("\n");
}

void Geometry::printVector(glm::vec3 v)
{
    std::cout << "<" << v.x << ", " << v.y << ", " << v.z << ">" << std::endl;
}


Geometry::RaySurfaceIntersection::RaySurfaceIntersection(WaylandSurfaceNode *surfaceNode, glm::vec2 surfaceLocalCoordinates, const Geometry::Ray &ray, float t)
    : surfaceNode(surfaceNode)
    , surfaceLocalCoordinates(surfaceLocalCoordinates)
    , ray(ray)
    , t(t)
{
}



