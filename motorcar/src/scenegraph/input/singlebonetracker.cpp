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








