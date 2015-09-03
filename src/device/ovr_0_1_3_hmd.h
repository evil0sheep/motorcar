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

#ifndef OCULUSHMD_H
#define OCULUSHMD_H

#include <scenegraph/output/display/rendertotexturedisplay.h>
#include <scenegraph/input/singlebonetracker.h>
#include "OVR_Version.h"
#include "OVR_CAPI.h"
#include <glm/gtc/quaternion.hpp>

namespace motorcar {

class OculusHMD : public RenderToTextureDisplay
{
private:
    class OVRSystem;

public:

    //Attempts to create an OculusHMD through the API, if something in the API Fails
    //(for example if no HMD's are present) this method returns a NULL pointer
    static OculusHMD *create(OpenGLContext *glContext, Skeleton *skeleton, PhysicalNode *parent);
    ~OculusHMD();



    void prepareForDraw() override;

    //This constructor should not be called externally, use create() instead;
    OculusHMD(OVRSystem * system, Skeleton *skeleton,
              float scale, glm::vec4 distortionK, OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform);


private:

    SingleBoneTracker *m_boneTracker;


    class OVRSystem : OVR::MessageHandler{
    public:
        OVRSystem() : m_display(NULL) {m_isInitialized = initializeDevice();}
        OculusHMD *getDisplay(OpenGLContext *glContext, Skeleton *skeleton, PhysicalNode *parent);
         ~OVRSystem();

        OculusHMD *m_display;

        bool m_isInitialized;
        float m_eyeHeight;
        OVR::Ptr<OVR::DeviceManager>  pManager;
        OVR::Ptr<OVR::SensorDevice>   pSensor;
        OVR::Ptr<OVR::HMDDevice>      pHMD;
        OVR::Ptr<OVR::Profile>        pUserProfile;

        OVR::SensorFusion             SFusion;
        OVR::HMDInfo                  TheHMDInfo;

        OVR::Util::Render::StereoConfig  SConfig;

        bool initializeDevice();


        //inherited from MessageHandler
           virtual void 	OnMessage (const OVR::Message &msg) override ;
           virtual bool 	SupportsMessageType (OVR::MessageType mt) const override;
    };

    OVRSystem *m_system;



};
}


#endif // OCULUSHMD_H
