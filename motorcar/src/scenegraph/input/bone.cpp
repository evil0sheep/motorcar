#include "bone.h"

using namespace motorcar;

motorcar::Bone::Bone(motorcar::PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
{

}


void Bone::setPosition(const glm::vec3 &position)
{
    //setTransform(transform());
}


void Bone::setOrientation(const glm::quat &orientation)
{
    //m_orientation = orientation;
}



