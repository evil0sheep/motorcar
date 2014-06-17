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
#include "oculushmd.h"

using namespace motorcar;
using namespace OVR;

//using namespace OVR::Platform;
//using namespace OVR::Render;

OculusHMD::OculusHMD(OVRSystem *system, Skeleton *skeleton, float scale, glm::vec4 distortionK, OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform)
    :RenderToTextureDisplay(scale, distortionK, glContext, displayDimensions, parent, transform)
    , m_boneTracker(new SingleBoneTracker(skeleton->headBone(), glm::mat4(),skeleton, parent))
    , m_system(system)
{
}



OculusHMD::~OculusHMD()
{
    delete m_system;
    OVR::System::Destroy();
}



void OculusHMD::prepareForDraw()
{
    OVR::Quatf ovrQuat = m_system->SFusion.GetOrientation();

    OVR::Vector3f OVRaxis;
    float angle;
    ovrQuat.GetAxisAngle(&OVRaxis, &angle);

    glm::vec3 axis =glm::vec3(OVRaxis.x, OVRaxis.y, OVRaxis.z);

    glm::quat orientation = glm::angleAxis(glm::degrees(angle), glm::normalize(axis));

    glm::vec3 parentPos = glm::vec3(parentNode()->worldTransform() * glm::vec4(0,0,0,1));

    m_boneTracker->setOrientation(glm::mat3_cast(orientation));


    RenderToTextureDisplay::prepareForDraw();
}


OculusHMD *OculusHMD::OVRSystem::getDisplay(OpenGLContext *glContext, Skeleton *skeleton, PhysicalNode *parent)
{

    if(m_display == NULL){

        int HResolution, VResolution ;
        float HScreenSize, VScreenSize, VScreenCenter,EyeToScreenDistance, LensSeparationDistance, InterpupillaryDistance, scaleFactor;
        glm::vec4 DistortionK;



        if(!m_isInitialized){
            //default Values
            HResolution = 1280;
            VResolution  = 800 ;
            HScreenSize = .14976;
            VScreenSize = .0936;
            VScreenCenter = VScreenSize/2;
            EyeToScreenDistance = 0.041f;
            LensSeparationDistance = .064;
            InterpupillaryDistance = .0647;
            DistortionK = glm::vec4(1, .22, .24, 0);
            scaleFactor = 1.25;
            return NULL;
        }else{
             std::cout << "using HMD Parameters from API" << std::endl;

            if(TheHMDInfo.HResolution > 0)
            {
                HResolution  = TheHMDInfo.HResolution;
                VResolution = TheHMDInfo.VResolution;
            }else{
                return NULL;
            }

            HScreenSize = TheHMDInfo.HScreenSize;
            VScreenSize = TheHMDInfo.VScreenSize;
            VScreenCenter = TheHMDInfo.VScreenCenter;
            EyeToScreenDistance = TheHMDInfo.EyeToScreenDistance;
            LensSeparationDistance = TheHMDInfo.LensSeparationDistance;
            InterpupillaryDistance = TheHMDInfo.InterpupillaryDistance;

            DistortionK = glm::make_vec4(TheHMDInfo.DistortionK);
            scaleFactor = SConfig.GetDistortionScale();

            std::cout << "mag yaw calibration enabled: " << SFusion.IsYawCorrectionEnabled() << std::endl;

            SFusion.SetYawCorrectionEnabled(true);
            std::cout << "mag yaw calibration enabled: " << SFusion.IsYawCorrectionEnabled() << std::endl;

        }
        std::cout << "IPD " << InterpupillaryDistance << std::endl;


        float near = .01, far = 10;

        float h_meters = HScreenSize / 4 - LensSeparationDistance / 2;
        float h = (4 * h_meters) / HScreenSize ;

        //RenderToTextureDisplay display(scaleFactor, DistortionK, glContext, glm::vec2(HScreenSize, VScreenSize), parent, glm::translate(glm::mat4(), glm::vec3(0, 0, EyeToScreenDistance)));

        m_display = new OculusHMD(this, skeleton, scaleFactor, DistortionK, glContext, glm::vec2(HScreenSize, VScreenSize), parent, glm::translate(glm::mat4(), glm::vec3(0, 0, EyeToScreenDistance)));



        ViewPoint *lCam = new ViewPoint(near, far, m_display, m_display,
                                       glm::translate(glm::mat4(), glm::vec3(-InterpupillaryDistance/2, VScreenSize/2 - VScreenCenter, EyeToScreenDistance)),
                                       glm::vec4(0,0,.5,1), glm::vec3(h, 0, 0));

        ViewPoint *rCam = new ViewPoint(near, far, m_display, m_display,
                                       glm::translate(glm::mat4(), glm::vec3(InterpupillaryDistance/2, VScreenSize/2 - VScreenCenter, EyeToScreenDistance)),
                                       glm::vec4(.5,0,.5,1), glm::vec3(-h, 0, 0));

        m_display->addViewpoint(lCam);
        m_display->addViewpoint(rCam);



    }

    return m_display;

}

OculusHMD::OVRSystem::~OVRSystem()
{
    if(pManager){
        pManager.Clear();
        pManager->Release();
    }
    if(pSensor){
        pSensor.Clear();
        pSensor->Release();
    }
    if(pHMD) pHMD->Release();
    if(pUserProfile) pUserProfile->Release();



}



bool OculusHMD::OVRSystem::initializeDevice()
{
    std::cout << "Attempting to intitialize devices" << std::endl;

    pManager = *DeviceManager::Create();

    if(!pManager){
        std::cout << "failed to create DeviceManager: "<< pManager << std::endl;
        return false;
    }


       // We'll handle it's messages in this case.
       pManager->SetMessageHandler(this);


       pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
       if (pHMD)
       {
           pSensor = *pHMD->GetSensor();
           SFusion.AttachToSensor(pSensor);

           // This will initialize HMDInfo with information about configured IPD,
           // screen size and other variables needed for correct projection.
           // We pass HMD DisplayDeviceName into the renderer to select the
           // correct monitor in full-screen mode.
           if(pHMD->GetDeviceInfo(&TheHMDInfo))
           {
               //RenderParams.MonitorName = hmd.DisplayDeviceName;
               SConfig.SetHMDInfo(TheHMDInfo);
           }

           // Retrieve relevant profile settings.
           pUserProfile = pHMD->GetProfile();
           if (pUserProfile)
           {
               std::cout << "using profile with ipd: "<< pUserProfile->GetIPD() << std::endl;
               m_eyeHeight = pUserProfile->GetEyeHeight();

           }
       }
       else
       {
           // If we didn't detect an HMD, try to create the sensor directly.
           // This is useful for debugging sensor interaction; it is not needed in
           // a shipping app.
           pSensor = *pManager->EnumerateDevices<SensorDevice>().CreateDevice();

           std::cout << "Could not enumerate Oculus HMD Device" << std::endl;
           return false;
       }

       return true;
}

void OculusHMD::OVRSystem::OnMessage(const Message &msg)
{
    if (msg.Type == Message_DeviceAdded || msg.Type == Message_DeviceRemoved)
       {
           if (msg.pDevice == pManager)
           {
               const MessageDeviceStatus& statusMsg =
                   static_cast<const MessageDeviceStatus&>(msg);

               { // limit the scope of the lock
                   Lock::Locker lock(pManager->GetHandlerLock());
//                   DeviceStatusNotificationsQueue.PushBack(
//                       DeviceStatusNotificationDesc(statusMsg.Type, statusMsg.Handle));
               }

               switch (statusMsg.Type)
               {
                   case OVR::Message_DeviceAdded:
                       LogText("DeviceManager reported device added.\n");
                       break;

                   case OVR::Message_DeviceRemoved:
                       LogText("DeviceManager reported device removed.\n");
                       break;

                   default: OVR_ASSERT(0); // unexpected type
               }
           }
       }

}

bool OculusHMD::OVRSystem::SupportsMessageType(MessageType mt) const
{
    return true;
}

OculusHMD *OculusHMD::create(OpenGLContext *glContext, Skeleton *skeleton, PhysicalNode *parent)
{
    System::Init();
    OVRSystem *system = new OVRSystem();
    return system->getDisplay(glContext, skeleton, parent);
}








