#include "bonesensor.h"

using namespace motorcar;

BoneSensor::BoneSensor(Skeleton *skeleton, PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
{
    setSkeleton(skeleton);
}
Skeleton *BoneSensor::skeleton() const
{
    return m_skeleton;
}

void BoneSensor::setSkeleton(Skeleton *skeleton)
{
    if(skeleton == NULL){
        std::cout << "Setting bone sensor skeleton NULL, results undefined" << std::endl;
    }

    m_skeleton = skeleton;
}

