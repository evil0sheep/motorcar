#include "spatialpointingdevice.h"
#include "../scene.h"

using namespace motorcar;

SpatialPointingDevice::SpatialPointingDevice(PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
    ,m_latestIntersection(NULL)
    ,m_leftMouseDown(false)
    ,m_rightMouseDown(false)
    ,m_middleMouseDown(false)
    ,m_grabbedSurface(NULL)
    ,m_grabbedSurfaceParent(NULL)
{ 
    float vertices[]= {
        0, 0, 0,
        0.1, 0, 0,
        0, 0, 0,
        0, 0.1, 0,
        0, 0, 0,
        0, 0, -10,

    };
    new WireframeNode(vertices, 3, glm::vec3(1,0,0), this);

    //new WireframeNode(vertices, 3, glm::vec3(0,1,0), this, glm::rotate(glm::mat4(), -45.f, glm::vec3(1,0,0)));


}

void SpatialPointingDevice::traverseNode(Scene *scene, long deltaMillis)
{
    PhysicalNode::traverseNode(scene, deltaMillis);

    Geometry::Ray ray = Geometry::Ray(glm::vec3(0,0,0), glm::vec3(0,0,-1)).transform(worldTransform());

    Geometry::RaySurfaceIntersection *intersection = scene->intersectWithSurfaces(ray);

    this->m_latestIntersection = intersection;




}



void SpatialPointingDevice::grabSurfaceUnderCursor()
{
    //std::cout << "attempting to grab surface" <<std::endl;
    if(m_grabbedSurface == NULL && m_latestIntersection != NULL){
        WaylandSurfaceNode *node = m_latestIntersection->surfaceNode;
        m_grabbedSurfaceParent = node->parentNode();
        node->setParentNode(this);
        node->setTransform(this->inverseWorldTransform() * m_grabbedSurfaceParent->worldTransform() * node->transform());
        m_grabbedSurface = node;
    }
}

void SpatialPointingDevice::releaseGrabbedSurface()
{
    //std::cout << "attempting to release grabbed surface" <<std::endl;
    if(m_grabbedSurface != NULL){
        m_grabbedSurface->setParentNode(m_grabbedSurfaceParent);
        m_grabbedSurface->setTransform(this->worldTransform() * m_grabbedSurfaceParent->inverseWorldTransform() * m_grabbedSurface->transform());
        m_grabbedSurface = NULL;
    }
}

Geometry::RaySurfaceIntersection *SpatialPointingDevice::latestIntersection() const
{
    return m_latestIntersection;
}




bool SpatialPointingDevice::rightMouseDown() const
{
    return m_rightMouseDown;
}

void SpatialPointingDevice::setRightMouseDown(bool rightMouseDown)
{
    m_rightMouseDown = rightMouseDown;
}

bool SpatialPointingDevice::leftMouseDown() const
{
    return m_leftMouseDown;
}

void SpatialPointingDevice::setLeftMouseDown(bool leftMouseDown)
{
    m_leftMouseDown = leftMouseDown;
}

bool SpatialPointingDevice::middleMouseDown() const
{
    return m_middleMouseDown;
}

void SpatialPointingDevice::setMiddleMouseDown(bool middleMouseDown)
{
    m_middleMouseDown = middleMouseDown;
}




