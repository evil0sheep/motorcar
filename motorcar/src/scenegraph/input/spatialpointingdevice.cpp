#include "spatialpointingdevice.h"
#include "../scene.h"

using namespace motorcar;

SpatialPointingDevice::SpatialPointingDevice(PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
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
}

void SpatialPointingDevice::traverseNode(Scene *scene, long deltaMillis)
{
    PhysicalNode::traverseNode(scene, deltaMillis);

    Geometry::Ray ray = Geometry::Ray(glm::vec3(0), glm::vec3(0,0,-1)).transform(worldTransform());

    Geometry::RaySurfaceIntersection *intersection = scene->intersectWithSurfaces(ray);
}
