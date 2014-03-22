#include "sixensecontrollernode.h"
using namespace motorcar;

#include "../scenegraph/scene.h"

SixenseControllerNode::SixenseControllerNode(int controllerIndex, PhysicalNode *parent, const glm::mat4 &transform )
    :PhysicalNode(parent, transform)
    ,m_pointingDevice(NULL)
    ,m_controllerIndex(controllerIndex)
    ,m_enabled(true)
    ,m_bumperDown(false)
    ,m_filteredPos(0)
    ,m_filterConstant(.8)
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

        m_pointingDevice->setLeftMouseDown((data.buttons & SIXENSE_BUTTON_1) != 0);
        m_pointingDevice->setRightMouseDown((data.buttons & SIXENSE_BUTTON_2) != 0);
        m_pointingDevice->setMiddleMouseDown((data.buttons & SIXENSE_BUTTON_JOYSTICK) != 0);
    }

    glm::mat3 rotation = glm::make_mat3((float *)data.rot_mat);
    glm::vec3 newPosition = (glm::make_vec3(data.pos) / 1000.f);

    m_filteredPos = (1-m_filterConstant) * m_filteredPos + m_filterConstant * newPosition;


//                if(controller->controllerIndex() == 0){
//                    Geometry::Ray(glm::vec3(0), rotation * glm::vec3(0,1,0)).draw(this, glm::vec3(1, 0, 1));

//                }



    glm::mat4 controllerTransform = glm::translate(glm::mat4(), m_filteredPos);// *
//                                    glm::mat4(glm::vec4(rotation[0], 0),
//                                              glm::vec4(rotation[1], 0),
//                                              glm::vec4(rotation[2], 0),
//                                              glm::vec4(0,0,0, 1));
    if(controllerIndex() != 0){
        controllerTransform = controllerTransform * glm::mat4(glm::vec4(rotation[0], 0),
                                                              glm::vec4(rotation[1], 0),
                                                              glm::vec4(rotation[2], 0),
                                                              glm::vec4(0,0,0, 1));
    }

//                if(controller->controllerIndex() != 0){
//                    controller->setWorldTransform(glm::translate(glm::mat4(), glm::vec3(0,0.1,0)));
//                else
        //controller->setWorldTransform(this->inverseWorldTransform() * controllerTransform);
    setTransform(controllerTransform);

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
