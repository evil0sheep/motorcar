#include "waylandsurfacenode.h"
#include "../display/display.h"
using namespace motorcar;

WaylandSurfaceNode::WaylandSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform)
    :WaylandDrawable(parent, transform)
{
    this->setSurface(surface);

}

WaylandSurface *WaylandSurfaceNode::surface() const
{
    return m_surface;
}

void WaylandSurfaceNode::setSurface(WaylandSurface *surface)
{
    m_surface = surface;
}

glm::mat4 WaylandSurfaceNode::surfaceTransform() const
{
    return m_surfaceTransform;
}

void WaylandSurfaceNode::computeSurfaceTransform(float ppcm)
{
    if(ppcm > 0){
        float ppm = ppcm * 100.f;
        glm::mat4 surfaceRotation = glm::rotate(glm::mat4(1), 180.f ,glm::vec3(0, 0, 1));
        glm::mat4 surfaceScale = glm::scale(glm::mat4(1), glm::vec3( -m_surface->size().x / ppm,  m_surface->size().y / ppm, 1));
        glm::mat4 surfaceOffset = glm::translate(glm::mat4(1), glm::vec3(-0.5f, -0.5f, 0));
        m_surfaceTransform = surfaceRotation * surfaceScale * surfaceOffset  ;
    }
}

bool WaylandSurfaceNode::computeLocalSurfaceIntersection(const Geometry::Ray &localRay, glm::vec2 &localIntersection, float &t) const
{
    Geometry::Plane surfacePlane = Geometry::Plane(glm::vec3(0), glm::vec3(0,0,1));
    if(glm::dot(localRay.d, surfacePlane.n) == 0) return false;

    Geometry::Ray transformedRay = localRay.transform(glm::inverse(surfaceTransform()));

    t = surfacePlane.intersect(transformedRay);
    //std::cout << "t = " << t << std::endl;
    glm::vec3 intersection = transformedRay.solve(t);

    Geometry::printVector(glm::vec3(intersection));
    glm::vec3 coords= intersection * glm::vec3(m_surface->size().x, m_surface->size().y, 0);
    localIntersection =  glm::vec2(coords);

    return true;
}

Geometry::RaySurfaceIntersection *WaylandSurfaceNode::intersectWithSurfaces(const Geometry::Ray &ray)
{
    Geometry::RaySurfaceIntersection *closestSubtreeIntersection = SceneGraphNode::intersectWithSurfaces(ray);
    Geometry::Ray localRay = ray.transform(inverseTransform());

    float t;
    glm::vec2 localIntersection;
    bool isIntersected = computeLocalSurfaceIntersection(localRay, localIntersection, t);


    if(isIntersected && (closestSubtreeIntersection == NULL || t < closestSubtreeIntersection-> t)){


        if(localIntersection.x >= 0 && localIntersection.x <= m_surface->size().x && localIntersection.y >= 0 && localIntersection.y <= m_surface->size().y){
            return new Geometry::RaySurfaceIntersection(this, localIntersection, ray, t);
        }else{
            return NULL;
        }

    }else{
        return closestSubtreeIntersection;
    }
}

void WaylandSurfaceNode::drawViewpoint(GLCamera *viewpoint)
{
    //std::cout << "drawing surface" <<std::endl;
    computeSurfaceTransform(32);
    surface()->prepare();
    viewpoint->viewport()->display()->renderSurfaceNode(this, viewpoint);
}
