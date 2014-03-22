#include "singlebonetracker.h"
using namespace motorcar;

SingleBoneTracker::SingleBoneTracker(Bone *trackedBone, glm::mat4 boneTrackTransform, Skeleton *skeleton, PhysicalNode *parent, const glm::mat4 &transform)
    :BoneSensor(skeleton, parent, transform)
    ,m_trackedBone(trackedBone)
    ,m_boneTrackTransform(boneTrackTransform)
{
}

void SingleBoneTracker::setTransform(const glm::mat4 &transform)
{

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




