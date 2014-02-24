#include "sixensecontrollernode.h"
using namespace motorcar;

#include "../scenegraph/scene.h"

SixenseControllerNode::SixenseControllerNode(int controllerIndex, PhysicalNode *parent, const glm::mat4 &transform )
    :SpatialPointingDevice(parent, transform)
    ,m_controllerIndex(controllerIndex)
    ,m_enabled(true)
{
}

void SixenseControllerNode::updateState(sixenseControllerData data)
{
    if(data.buttons & SIXENSE_BUTTON_1){
        //std::cout << "test" <<std::endl;

        glm::mat4 T = worldTransform();
        Geometry::Ray ray = Geometry::Ray(glm::vec3(0,0,0), glm::vec3(0,0,-1));
        Geometry::Ray tranformedRay = ray.transform(worldTransform());

          ray.draw(this, glm::vec3(0,1,0));

//        Geometry::printVector(ray.d);
        Geometry::RaySurfaceIntersection *intersection = scene()->intersectWithSurfaces(tranformedRay);

//        if(intersection){
//            Geometry::printVector(glm::vec3(intersection->surfaceLocalCoordinates, 0));
//        }

    }
}

int SixenseControllerNode::controllerIndex() const
{
    return m_controllerIndex;
}

void SixenseControllerNode::setControllerIndex(int controllerIndex)
{
    m_controllerIndex = controllerIndex;
}

void SixenseControllerNode::traverseNode(Scene *scene, long deltaMillis)
{
    SpatialPointingDevice::traverseNode(scene, deltaMillis);
//    std::cout << "controller " << controllerIndex() << " has " << childNodes().size()<< " children" <<std::endl;
//    Geometry::printMatrix(worldTransform());
}



bool SixenseControllerNode::enabled() const
{
    return m_enabled;
}

void SixenseControllerNode::setEnabled(bool enabled)
{
    m_enabled = enabled;
}
