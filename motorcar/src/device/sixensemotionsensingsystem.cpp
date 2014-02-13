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


//        int maxBases = sixenseGetMaxBases();
//        for(int i =0; i < maxBases; i++){
//            if(sixenseSetActiveBase(i) && sixenseIsBaseConnected(i)){
//                m_baseStations.push_back(new SixenseBaseNode(i));
//            }
//        }

//        std::cout << "max base stations: " << maxBases << " num_bases: " << m_baseStations.size() << std::endl;
//        sixenseSetActiveBase(0);

//        if(!m_baseStations.empty()){
//            int maxControllers = sixenseGetMaxControllers();
//            for(int i =0; i < maxControllers; i++){
//                if(sixenseIsControllerEnabled(i)){
//                    m_controllers.push_back(new SixenseControllerNode(i, scene));
//                }
//            }
//            if(!m_controllers.empty()){
//                m_isInitialized = true;
//                std::cout << "SixenseMotionSensingSystem successfully initialized" << std::endl;
//            }else{
//                m_isInitialized = false;
//                std::cout << "Could not intitialize SixenseMotionSensingSystem: No Sixense Motion Controllers connected" << std::endl;
//            }
//        }else{
//            m_isInitialized = false;
//            std::cout << "Could not intitialize SixenseMotionSensingSystem: No Sixense Base Stations connected" << std::endl;
//        }


        int base, cont;
        sixenseAllControllerData acd;
        FILE *log_file = stdout;

        std::cout << "test" << std::endl;
        sleep(1);

            for( base=0; base < sixenseGetMaxBases(); base++ ) {
                sixenseSetActiveBase(base);

                sixenseGetAllNewestData( &acd );

                int numControllers = 0;

                for( cont=0; cont<sixenseGetMaxControllers(); cont++ ) {

                    if( sixenseIsControllerEnabled( cont ) ) {

                        fprintf( log_file, "base: %d controller: %d ", base, cont );

                        fprintf( log_file, "pos: %f %f %f ", acd.controllers[cont].pos[0], acd.controllers[cont].pos[1], acd.controllers[cont].pos[2] );
                        fprintf( log_file, "rot_mat: %f %f %f  %f %f %f  %f %f %f",
                            acd.controllers[cont].rot_mat[0][0], acd.controllers[cont].rot_mat[0][1], acd.controllers[cont].rot_mat[0][2],
                            acd.controllers[cont].rot_mat[1][0], acd.controllers[cont].rot_mat[1][1], acd.controllers[cont].rot_mat[1][2],
                            acd.controllers[cont].rot_mat[2][0], acd.controllers[cont].rot_mat[2][1], acd.controllers[cont].rot_mat[2][2] );

                        fprintf( log_file, "\n" );
                        fflush(log_file);

                        numControllers ++;
                        m_controllers.push_back(new SixenseControllerNode(cont, scene));


                    }

                }
                if(numControllers > 0){
                    m_baseStations.push_back(new SixenseBaseNode(base));
                }
            }



    }else{
        m_isInitialized = false;
        std::cout << "Could not intitialize SixenseMotionSensingSystem: Failed to initialize Sixsense SDK" << std::endl;

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
