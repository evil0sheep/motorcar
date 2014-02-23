#include "sixensemotionsensingsystem.h"

using namespace motorcar;

#include <unistd.h>

SixenseMotionSensingSystem::SixenseMotionSensingSystem(Scene *scene)
    :m_isInitialized(false)
{

    if(sixenseInit() == SIXENSE_SUCCESS){
        //SixenseMotionSensingSystem *system = new SixenseMotionSensingSystem() ;
        // Init the controller manager. This makes sure the controllers are present, assigned to left and right hands, and that
        // the hemisphere calibration is complete.
        sixenseUtils::getTheControllerManager()->setGameType( sixenseUtils::ControllerManager::ONE_PLAYER_TWO_CONTROLLER );
        sixenseUtils::getTheControllerManager()->registerSetupCallback(*(SixenseMotionSensingSystem::controllerManagerSetupCallback));



        std::cout << "Sixense SDK Initialized, attempting to enumerate attached devices" << std::endl;
        sleep(1);

        for(int base=0; base < sixenseGetMaxBases(); base++ ) {
            if(sixenseIsBaseConnected(base)){

                std::cout << "Sixense base " << base << "is active, intitializing" << std::endl;

                m_baseStations.push_back(new SixenseBaseNode(base, scene));//, glm::rotate(glm::mat4(1), 180.f, glm::vec3(0,1,0))));

            }
        }
        if(m_baseStations.empty()){
            m_isInitialized = false;
            std::cout << "Could not intitialize SixenseMotionSensingSystem: No Sixense devices detected" << std::endl;
        }else{
            m_isInitialized = true;
            std::cout << "Successfully intitialize SixenseMotionSensingSystem" << std::endl;
        }



    }else{
        m_isInitialized = false;
        std::cout << "Failed to initialize Sixsense SDK" << std::endl;

    }

}

SixenseMotionSensingSystem::~SixenseMotionSensingSystem()
{
    sixenseExit();
}

void SixenseMotionSensingSystem::controllerManagerSetupCallback(sixenseUtils::IControllerManager::setup_step step)
{



    if( sixenseUtils::getTheControllerManager()->isMenuVisible() ) {

            // Turn on the flag that tells the graphics system to draw the instruction screen instead of the controller information. The game
            // should be paused at this time.
            //m_controllerSetupScreenVisible = true;

            // Ask the controller manager what the next instruction string should be.
            std::string controller_manager_text_string = sixenseUtils::getTheControllerManager()->getStepString();

            std::cout << "Sixense controller manager says: " << controller_manager_text_string << std::endl;

            // We could also load the supplied controllermanager textures using the filename: sixenseUtils::getTheControllerManager()->getTextureFileName();

     } else {

            // We're done with the setup, so hide the instruction screen.
           //m_controllerSetupScreenVisible = false;





    }
}

bool SixenseMotionSensingSystem::isInitialized() const
{
    return m_isInitialized;
}

std::vector<SixenseBaseNode *> SixenseMotionSensingSystem::baseStations() const
{
    return m_baseStations;
}


