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



