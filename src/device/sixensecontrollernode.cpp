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
#include "sixensecontrollernode.h"
using namespace motorcar;

#include <scenegraph/scene.h>
#include <compositor.h>

SixenseControllerNode::SixenseControllerNode(int controllerIndex, PhysicalNode *parent, const glm::mat4 &transform )
    :PhysicalNode(parent, transform)
    ,m_pointingDevice(NULL)
    ,m_boneTracker(NULL)
    ,m_controllerIndex(controllerIndex)
    ,m_enabled(true)
    ,m_bumperDown(false)
    ,m_filteredPos(0)
    ,m_filterConstant(.7)
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


    glm::mat4 controllerTransform = glm::translate(glm::mat4(), m_filteredPos);// *


    controllerTransform = glm::translate(glm::mat4(), m_filteredPos) * glm::mat4(glm::vec4(rotation[0], 0),
                                                              glm::vec4(rotation[1], 0),
                                                              glm::vec4(rotation[2], 0),
                                                              glm::vec4(0,0,0, 1));


    if(data.buttons & SIXENSE_BUTTON_START){
        printf("Calibrating sixense controller to display\n\tHold controller with joystick on chin\n\tand point controller at nose\n");
        glm::mat4 displayTransform = this->scene()->activeDisplay()->worldTransform();
        glm::mat4 targetTransform = displayTransform * glm::translate(glm::mat4(), glm::vec3(0, -0.15, 0.05)) 
                                                        * glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(1, 0, 0));
        glm::mat4 transformError = glm::inverse(targetTransform) * (this->parentNode()->worldTransform() * controllerTransform);

        m_tranformOffset = glm::inverse(transformError);

    }

    setTransform(controllerTransform * m_tranformOffset);


    if(m_boneTracker != NULL){
        m_boneTracker->setPosition(m_filteredPos);
        //m_boneTracker->setOrientation(rotation);
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





bool SixenseControllerNode::enabled() const
{
    return m_enabled;
}

void SixenseControllerNode::setEnabled(bool enabled)
{
    m_enabled = enabled;
}



SixDOFPointingDevice *SixenseControllerNode::pointingDevice() const
{
    return m_pointingDevice;
}

void SixenseControllerNode::setPointingDevice(SixDOFPointingDevice *pointingDevice)
{
    m_pointingDevice = pointingDevice;
}
SingleBoneTracker *SixenseControllerNode::boneTracker() const
{
    return m_boneTracker;
}

void SixenseControllerNode::setBoneTracker(SingleBoneTracker *boneTracker)
{
    m_boneTracker = boneTracker;
}

