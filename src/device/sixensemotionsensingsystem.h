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

#ifndef SIXENSEMOTIONSENSINGSYSTEM_H
#define SIXENSEMOTIONSENSINGSYSTEM_H
#include <sixense.h>
#include <sixense_utils/button_states.hpp>
#include <sixense_utils/event_triggers.hpp>
#include <sixense_utils/controller_manager/controller_manager.hpp>

#include "sixensecontrollernode.h"
#include "sixensebasenode.h"

#include <scenegraph/scene.h>

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
