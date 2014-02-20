#ifndef SIXENSEMOTIONSENSINGSYSTEM_H
#define SIXENSEMOTIONSENSINGSYSTEM_H
#include <sixense.h>
#include <sixense_utils/button_states.hpp>
#include <sixense_utils/event_triggers.hpp>
#include <sixense_utils/controller_manager/controller_manager.hpp>

#include "sixensecontrollernode.h"
#include "sixensebasenode.h"

#include "../scenegraph/scene.h"

namespace motorcar {
class SixenseMotionSensingSystem
{
public:
    SixenseMotionSensingSystem(Scene *scene);
    ~SixenseMotionSensingSystem();

    //callback for controller setup and calibration
    static void controllerManagerSetupCallback(sixenseUtils::ControllerManager::setup_step step);

    //will be true after construction if sixenseInit() succeeds and at least one base station and at least one controller is present during construction
    bool isInitialized() const;

    std::vector<SixenseBaseNode *> baseStations() const;

private:
    std::vector<SixenseBaseNode *> m_baseStations;
    bool m_controllerSetupScreenVisible;
    bool m_isInitialized;
};
}


#endif // SIXENSEMOTIONSENSINGSYSTEM_H
