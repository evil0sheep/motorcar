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

#include "sixensebasenode.h"

#include <iostream>

using namespace motorcar;
SixenseBaseNode::SixenseBaseNode(int baseIndex, PhysicalNode *parent,
                                 const glm::mat4 &transform)
    : PhysicalNode(parent, transform), m_baseIndex(baseIndex),
      m_connected(true) {

  sixenseAllControllerData acd;

  sixenseSetActiveBase(baseIndex);

  sixenseGetAllNewestData(&acd);

  for (int cont = 0; cont < sixenseGetMaxControllers(); cont++) {
    if (sixenseIsControllerEnabled(cont)) {

      std::cout << "base: " << baseIndex << " controller: " << cont << " "
                << "pos: " << acd.controllers[cont].pos[0] << " "
                << acd.controllers[cont].pos[1] << " "
                << acd.controllers[cont].pos[2] << " "
                << "rot_mat: " << acd.controllers[cont].rot_mat[0][0]
                << acd.controllers[cont].rot_mat[0][1] << " "
                << acd.controllers[cont].rot_mat[0][2] << "  "
                << acd.controllers[cont].rot_mat[1][0] << " "
                << acd.controllers[cont].rot_mat[1][1] << " "
                << acd.controllers[cont].rot_mat[1][2] << "  "
                << acd.controllers[cont].rot_mat[2][0] << " "
                << acd.controllers[cont].rot_mat[2][1] << " "
                << acd.controllers[cont].rot_mat[2][2] << std::endl;

      SixenseControllerNode *controller = new SixenseControllerNode(cont, this);

      m_controllers.push_back(controller);
    }
  }

  float vertices[] = { -0.0f, 0.0f,   0.0f, 0.1f, 0.0f, 0.0f,    0.0f, -0.0f, 0.0f,
                       0.0f,    0.1f, 0.0f, 0.0f,   0.0f, -0.0f, 0.0f, 0.0f,    0.1f, };
  new WireframeNode(vertices, 3, glm::vec3(0.25f), this);
}

void SixenseBaseNode::handleFrameBegin(Scene *scene) {
  PhysicalNode::handleFrameBegin(scene);

  sixenseAllControllerData acd;

  if (sixenseIsBaseConnected(m_baseIndex)) {
    if (!connected()) {
      setConnected(true);
      std::cout << "Sixsense base " << m_baseIndex << " reconnected"
                << std::endl;
    }
    sixenseSetActiveBase(m_baseIndex);
    sixenseGetAllNewestData(&acd);

    for (SixenseControllerNode *controller : m_controllers) {
      if (sixenseIsControllerEnabled(controller->controllerIndex())) {
        if (!controller->enabled()) {
          controller->setEnabled(true);
          std::cout << "Sixsense controller " << controller->controllerIndex()
                    << " re-enabled" << std::endl;
        }

        sixenseControllerData data =
            acd.controllers[controller->controllerIndex()];

        controller->updateState(data);

      } else if (controller->enabled()) {
        controller->setEnabled(false);
        std::cout << "Sixsense controller " << controller->controllerIndex()
                  << " disabled" << std::endl;
      }
    }

  } else if (connected()) {
    setConnected(false);
    std::cout << "Sixsense base " << m_baseIndex << " disconnected"
              << std::endl;
  }
}

bool SixenseBaseNode::connected() const { return m_connected; }

void SixenseBaseNode::setConnected(bool connected) { m_connected = connected; }

std::vector<SixenseControllerNode *> SixenseBaseNode::controllers() const {
  return m_controllers;
}

