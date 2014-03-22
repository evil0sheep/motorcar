/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Compositor.
**
** $QT_BEGIN_LICENSE:BSD$
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
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include "motorcar/src/motorcar.h"
#include "../qt/src/qtwaylandmotorcarcompositor.h"




int main(int argc, char *argv[])
{



    motorcar::Scene *scene = new motorcar::Scene();



    motorcar::Compositor *compositor = qtmotorcar::QtWaylandMotorcarCompositor::create(argc, argv, scene) ;





    motorcar::OpenGLContext *context = compositor->getContext();


    motorcar::OculusHMD *hmd = motorcar::OculusHMD::create(context, scene);

    if(hmd){
        std::cout << "Using Oculus Display" << std::endl;
        compositor->setDisplay(hmd);
    }else{
        std::cout << "Using Default Display" << std::endl;
        float camToDisplayDistance = 0.1;
        motorcar::Display *display = new motorcar::Display(context, glm::vec2(0.325, 0.1), scene, glm::translate(glm::mat4(1), glm::vec3(0, 0, -camToDisplayDistance)));
        display->addViewpoint(new motorcar::GLCamera( .01, 100, display, display, glm::translate(glm::mat4(1), glm::vec3(0, 0, camToDisplayDistance))));
        compositor->setDisplay(display);
    }


    scene->addDisplay(compositor->display());



    motorcar::SixenseMotionSensingSystem *sixense = new motorcar::SixenseMotionSensingSystem(scene);
    if(sixense->isInitialized() && !sixense->baseStations().empty() && !sixense->baseStations().front()->controllers().empty() ){

        std::cout << "parenting display to controller "<<std::endl;
        compositor->display()->setParentNode(sixense->baseStations().front()->controllers().front());


//        glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0,0,-.00));

//        glm::mat4 rotation = glm::rotate(glm::mat4(), -45.f, glm::vec3(1,0,0));

          glm::mat4 offsetTransform =
                  glm::translate(glm::mat4(), glm::vec3(0, 0, -0.19)) *
                  glm::rotate(glm::mat4(), 50.f, glm::vec3(-1, 0, 0)) *
                  glm::translate(glm::mat4(), glm::vec3(0, -.16, 0));

          glm::vec3 displayPosition = glm::vec3(0);//glm::vec3(offsetTransform * glm::vec4(0,0,0,1));

          glm::mat4 displayTransform = glm::translate(glm::mat4(), displayPosition);

          compositor->display()->setTransform(displayTransform);
    }


    int result = compositor->start();

    delete sixense;
    delete scene;

    return result;
}
