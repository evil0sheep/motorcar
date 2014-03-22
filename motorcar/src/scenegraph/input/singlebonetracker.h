#ifndef SINGLEBONETRACKER_H
#define SINGLEBONETRACKER_H

#include "bonesensor.h"

namespace motorcar {
class SingleBoneTracker : public BoneSensor
{
public:
    SingleBoneTracker(Bone *trackedBone, glm::mat4 boneTrackTransform, Skeleton *skeleton, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());

    ///Sets the Bone's transform in parent space
    /* overridden to also transform the tracked bone correctly*/
    void setTransform(const glm::mat4 &transform) override;

    ///Gets the bone that this tracker is attached to
    Bone *trackedBone() const;
    ///Sets the bone that this tracker is attached to
    void setTrackedBone(Bone *trackedBone);


    glm::mat4 boneTrackTransform() const;
    ///Sets the transform that describes how this tracker is attached to the bone it tracks
    /*This transform defines the tracker space within the tracked bone space,
     * similar to the way a SceneGraphNode's transform defines its space within it's parentSpace.
     * Bone space is rooted at the join that connects the bone to its parent bone*/
    void setBoneTrackTransform(const glm::mat4 &boneTrackTransform);

private:
    Bone * m_trackedBone;
    glm::mat4  m_boneTrackTransform;
};

}

#endif // SINGLEBONETRACKER_H
