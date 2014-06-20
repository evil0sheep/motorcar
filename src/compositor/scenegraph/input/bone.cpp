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
#include "bone.h"

using namespace motorcar;

motorcar::Bone::Bone(motorcar::PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
{

}


void Bone::setPosition(const glm::vec3 &position)
{
    glm::mat4 trans = transform();

    //make new transform with same orientation but new position
    glm::mat4 newTransform = glm::mat4(trans[0], trans[1], trans[2], glm::vec4(position, 1));

    setTransform(newTransform);
}


void Bone::setOrientation(const glm::mat3 &orientation)
{
    glm::mat4 trans = transform();
    //make new transform with same position but new orientation
    glm::mat4 newTransform = glm::mat4(glm::vec4(orientation[0], trans[0][3]),
            glm::vec4(orientation[1], trans[1][3]),
            glm::vec4(orientation[2], trans[2][3]),
            glm::vec4(trans[3]));
    setTransform(newTransform);
}



