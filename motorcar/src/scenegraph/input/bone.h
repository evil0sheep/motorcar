#ifndef BONE_H
#define BONE_H
#include "inputelement.h"
#include "../physicalnode.h"
#include "glm/gtc/quaternion.hpp"

namespace motorcar{
class Bone : public InputElement, public PhysicalNode
{
public:
    Bone(PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());


    ///Set the position of the bone in parent space
    /*Sets the bone's transform to be a new transform with the existing orientation and the specified postion*/
    void setPosition(const glm::vec3 &position);
    ///Set the orientation of the bone in parent space
    /*Sets the bone's transform to be a new transform with the existing postion and the specified orientation*/
    void setOrientation(const glm::mat3 &orientation);



};
}

#endif // BONE_H
