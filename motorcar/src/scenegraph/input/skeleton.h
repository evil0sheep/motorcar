#ifndef SKELETON_H
#define SKELETON_H

#include "bone.h"

namespace motorcar{

class Skeleton : public InputElement, public PhysicalNode
{
public:
    Skeleton(PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());



    Bone *headBone() const;
    void setHeadBone(Bone *headBone);

private:
    Bone *m_headBone;

};

}

#endif // SKELETON_H
