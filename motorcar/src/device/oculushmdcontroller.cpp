#include "oculushmdcontroller.h"

using namespace motorcar;

OculusHMDController::OculusHMDController()
{
    m_display = NULL;
}

Display *OculusHMDController::getDisplay(OpenGLContext *glContext, PhysicalNode &parent)
{
    if(m_display == NULL){


        float HScreenSize = .14976,
              VScreenSize = .0936,
              VScreenCenter = VScreenSize/2,
              EyeToScreenDistance = 0.041f,
              LensSeparationDistance = .064,
              InterpupillaryDistance = .0647;
        int HResolution = 1280 , VResolution  = 800 ;
        glm::vec4 DistortionK = glm::vec4(1, .22, .24, 0);
        float scaleFactor = 1.25;

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


