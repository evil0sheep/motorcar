#ifndef OCULUSHMDCONTROLLER_H
#define OCULUSHMDCONTROLLER_H

#include "../scenegraph/output/display/rendertotexturedisplay.h"
#include "OVR.h"

namespace motorcar {

class OculusHMDController : OVR::MessageHandler
{
public:
    OculusHMDController();
    ~OculusHMDController();
    Display *getDisplay(OpenGLContext *glContext, PhysicalNode &parent);

//    //inherited from MessageHandler
    virtual void 	OnMessage (const OVR::Message &msg) override ;
    virtual bool 	SupportsMessageType (OVR::MessageType mt) const override;

    static OculusHMDController *create();

    bool isInitialized() const;

    void updatePosition();

private:
    RenderToTextureDisplay *m_display;
    bool m_isInitialized;
    float m_eyeHeight;

    bool initializeDevice();
    OVR::Ptr<OVR::DeviceManager>  pManager;
    OVR::Ptr<OVR::SensorDevice>   pSensor;
    OVR::Ptr<OVR::HMDDevice>      pHMD;
    OVR::Ptr<OVR::Profile>        pUserProfile;

    OVR::SensorFusion        SFusion;
    OVR::HMDInfo             TheHMDInfo;

    OVR::Util::Render::StereoConfig        SConfig;
//    OVR::Util::Render::PostProcessType     PostProcess;
};
}


#endif // OCULUSHMDCONTROLLER_H
