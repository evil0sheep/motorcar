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


    void setPosition(const glm::vec3 &position);
    void setOrientation(const glm::quat &orientation);



};
}

#endif // BONE_H
