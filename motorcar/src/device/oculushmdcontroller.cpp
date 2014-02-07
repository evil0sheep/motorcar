#include "oculushmdcontroller.h"

using namespace motorcar;
using namespace OVR;
//using namespace OVR::Platform;
//using namespace OVR::Render;

OculusHMDController::OculusHMDController()
    :m_display(NULL)
    ,m_isInitialized(false)
{
 m_isInitialized = initializeDevice();

}

OculusHMDController::~OculusHMDController()
{
    if(pManager) pManager->Release();
    OVR::System::Destroy();
}


Display *OculusHMDController::getDisplay(OpenGLContext *glContext, PhysicalNode &parent)
{

    if(m_display == NULL){

        int HResolution, VResolution ;
        float HScreenSize, VScreenSize, VScreenCenter,EyeToScreenDistance, LensSeparationDistance, InterpupillaryDistance, scaleFactor;
        glm::vec4 DistortionK;



        if(!this->isInitialized()){
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

        }



        float near = .01, far = 100;

        float h_meters = HScreenSize / 4 - LensSeparationDistance / 2;
        float h = (4 * h_meters) / HScreenSize ;


        m_display = new RenderToTextureDisplay(scaleFactor, DistortionK, glContext, glm::vec2(HScreenSize, VScreenSize), parent, glm::translate(glm::mat4(), glm::vec3(0, 0, EyeToScreenDistance)));

        GLCamera *lCam = new GLCamera(near, far, m_display, *m_display,
                                       glm::translate(glm::mat4(), glm::vec3(InterpupillaryDistance/2, VScreenSize/2 - VScreenCenter, -EyeToScreenDistance)),
                                       glm::vec4(0,0,.5,1), glm::vec3(h, 0, 0));

        GLCamera *rCam = new GLCamera(near, far, m_display, *m_display,
                                       glm::translate(glm::mat4(), glm::vec3(-InterpupillaryDistance/2, VScreenSize/2 - VScreenCenter, -EyeToScreenDistance)),
                                       glm::vec4(.5,0,.5,1), glm::vec3(-h, 0, 0));

        m_display->addViewpoint(lCam);
        m_display->addViewpoint(rCam);


    }

    return m_display;

}



bool OculusHMDController::initializeDevice()
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

void OculusHMDController::OnMessage(const Message &msg)
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

bool OculusHMDController::SupportsMessageType(MessageType mt) const
{
    return true;
}

OculusHMDController *OculusHMDController::create()
{
    System::Init();
    return new OculusHMDController();
}



bool OculusHMDController::isInitialized() const
{
    return m_isInitialized;
}



