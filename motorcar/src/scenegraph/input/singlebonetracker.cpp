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
#include "singlebonetracker.h"
using namespace motorcar;

SingleBoneTracker::SingleBoneTracker(Bone *trackedBone, glm::mat4 boneTrackTransform, Skeleton *skeleton, PhysicalNode *parent, const glm::mat4 &transform)
    :BoneSensor(skeleton, parent, transform)
    ,m_trackedBone(trackedBone)
    ,m_boneTrackTransform(boneTrackTransform)
{
}


glm::mat4 SingleBoneTracker::trackerParentSpaceToBoneParentSpaceTransform()
{
    //define tracker parent space in bone space
    glm::mat4 workingTransform = trackedBone()->inverseWorldTransform() * this->parentNode()->worldTransform();
    //apply inverse bone track transform in the bone space
    //this makes sure that the bone's orientation is taken into account when applying translation
    workingTransform = glm::inverse(boneTrackTransform()) * workingTransform;
    //bring into bone parent space
    workingTransform = trackedBone()->transform() * workingTransform;



    return workingTransform;
}

void SingleBoneTracker::setTransformFromTrackedBone()
{
    this->setTransform(this->parentNode()->inverseWorldTransform() * trackedBone()->worldTransform() * boneTrackTransform());
}

void SingleBoneTracker::setPosition(const glm::vec3 &position)
{
    glm::vec4 parentSpaceBonePos = trackerParentSpaceToBoneParentSpaceTransform() * glm::vec4(position, 1);

    trackedBone()->setPosition(glm::vec3(parentSpaceBonePos));

    setTransformFromTrackedBone();
}

void SingleBoneTracker::setOrientation(const glm::mat3 &orientation)
{
    glm::mat4 orientation4X4 = glm::mat4(orientation);
    glm::mat4 boneOrientation4X4 = trackerParentSpaceToBoneParentSpaceTransform() * orientation4X4 ;
    trackedBone()->setOrientation(glm::mat3(boneOrientation4X4));
    setTransformFromTrackedBone();



}



Bone *SingleBoneTracker::trackedBone() const
{
    return m_trackedBone;
}

void SingleBoneTracker::setTrackedBone(Bone *trackedBone)
{
    m_trackedBone = trackedBone;
}
glm::mat4 SingleBoneTracker::boneTrackTransform() const
{
    return m_boneTrackTransform;
}

void SingleBoneTracker::setBoneTrackTransform(const glm::mat4 &boneTrackTransform)
{
    m_boneTrackTransform = boneTrackTransform;
}








