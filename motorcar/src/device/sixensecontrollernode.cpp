#include "sixensecontrollernode.h"
using namespace motorcar;

SixenseControllerNode::SixenseControllerNode(int controllerIndex, PhysicalNode *parent, const glm::mat4 &transform )
    :SpatialPointingDevice(parent, transform)
    ,m_controllerIndex(controllerIndex)
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
    printStatus();
}

void SixenseControllerNode::printStatus()
{
    std::cout << "SixenseControllerNode " << m_controllerIndex << std::endl;
}
