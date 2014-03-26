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
