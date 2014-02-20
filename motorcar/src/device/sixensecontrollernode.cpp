#include "sixensecontrollernode.h"
using namespace motorcar;

SixenseControllerNode::SixenseControllerNode(int controllerIndex, PhysicalNode *parent, const glm::mat4 &transform )
    :SpatialPointingDevice(parent, transform)
    ,m_controllerIndex(controllerIndex)
    ,m_enabled(true)
{
}

void SixenseControllerNode::updateState(sixenseControllerData data)
{
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
