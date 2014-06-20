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

#ifndef SINGLEBONETRACKER_H
#define SINGLEBONETRACKER_H

#include "bonesensor.h"

namespace motorcar {
class SingleBoneTracker : public BoneSensor
{
public:
    SingleBoneTracker(Bone *trackedBone, glm::mat4 boneTrackTransform, Skeleton *skeleton, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());

    //Sets the tracked bone's transform directly from this transform
    //void setTransform(const glm::mat4 &transform) override;

    ///Set the position of the tracker in parent space
    /*Computes the tracked bone position based on the boneTrackTransform
     * and calls the equivilent method on the tracked bone with the computed position
     * then updates its own tranform based on the transform of the tracked bone
     * Note: only guarunteed to be correct if bone's orientation is correct for this frame
    */
    void setPosition(const glm::vec3 &position);

    ///Set the orientation of the tracker in parent space
    /*Computes the tracked bone orientation based on the boneTrackTransform
    * and calls the equivilent method on the tracked bone with the computed orientation
    * then updates its own tranform based on the transform of the tracked bone
   */
    void setOrientation(const glm::mat3 &orientation);

    ///Gets the bone that this tracker is attached to
    Bone *trackedBone() const;
    ///Sets the bone that this tracker is attached to
    void setTrackedBone(Bone *trackedBone);

    ///Gets the transform that describes how this tracker is attached to the bone it tracks
    glm::mat4 boneTrackTransform() const;
    ///Sets the transform that describes how this tracker is attached to the bone it tracks
    /*This transform defines the tracker space within the tracked bone space,
     * similar to the way a SceneGraphNode's transform defines its space within it's parentSpace.
     * Bone space is rooted at the join that connects the bone to its parent bone*/
    void setBoneTrackTransform(const glm::mat4 &boneTrackTransform);



private:
    Bone * m_trackedBone;
    glm::mat4  m_boneTrackTransform;
    glm::mat4 trackerParentSpaceToBoneParentSpaceTransform();
    void setTransformFromTrackedBone();
};

}

#endif // SINGLEBONETRACKER_H
