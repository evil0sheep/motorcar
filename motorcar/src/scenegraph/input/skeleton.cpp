#include "skeleton.h"

using namespace motorcar;

Skeleton::Skeleton(PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
{
    m_headBone = new Bone(this);
}
Bone *Skeleton::headBone() const
{
    return m_headBone;
}

void Skeleton::setHeadBone(Bone *headBone)
{
    m_headBone = headBone;
}




