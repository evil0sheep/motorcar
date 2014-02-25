#include "sixensecontrollernode.h"
using namespace motorcar;

#include "../scenegraph/scene.h"

SixenseControllerNode::SixenseControllerNode(int controllerIndex, PhysicalNode *parent, const glm::mat4 &transform )
    :PhysicalNode(parent, transform)
    ,m_pointingDevice(NULL)
    ,m_controllerIndex(controllerIndex)
    ,m_enabled(true)
    ,m_bumperDown(false)
{

}

void SixenseControllerNode::updateState(sixenseControllerData data)
{
    if(m_pointingDevice != NULL){
        if(data.buttons & SIXENSE_BUTTON_BUMPER){
            if(!m_bumperDown){
                m_bumperDown = true;
                m_pointingDevice->grabSurfaceUnderCursor();
            }
        }else{
            if(m_bumperDown){
                m_bumperDown = false;
                m_pointingDevice->releaseGrabbedSurface();
            }
        }
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
   PhysicalNode::traverseNode(scene, deltaMillis);
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



SpatialPointingDevice *SixenseControllerNode::pointingDevice() const
{
    return m_pointingDevice;
}

void SixenseControllerNode::setPointingDevice(SpatialPointingDevice *pointingDevice)
{
    m_pointingDevice = pointingDevice;
}
