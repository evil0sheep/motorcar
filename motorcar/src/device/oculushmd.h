#ifndef OCULUSHMD_H
#define OCULUSHMD_H

#include "../scenegraph/output/display/rendertotexturedisplay.h"
#include "OVR.h"

namespace motorcar {

class OculusHMD : public RenderToTextureDisplay
{
private:
    class OVRSystem;

public:

    //Attempts to create an OculusHMD through the API, if something in the API Fails
    //(for example if no HMD's are present) this method returns a NULL pointer
    static OculusHMD *create(OpenGLContext *glContext, PhysicalNode *parent);
    ~OculusHMD();



    void prepareForDraw() override;

    //This constructor should not be called externally, use create() instead;
    OculusHMD(OVRSystem * system,
              float scale, glm::vec4 distortionK, OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform);

private:




    class OVRSystem : OVR::MessageHandler{
    public:
        OVRSystem() : m_display(NULL) {m_isInitialized = initializeDevice();}
        OculusHMD *getDisplay(OpenGLContext *glContext, PhysicalNode *parent);
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
