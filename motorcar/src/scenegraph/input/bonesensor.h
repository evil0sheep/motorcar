#ifndef BONESENSOR_H
#define BONESENSOR_H

#include "inputelement.h"
#include "../physicalnode.h"
#include "skeleton.h"

namespace motorcar{
class BoneSensor : public InputElement, public PhysicalNode
{
public:
    BoneSensor(Skeleton *skeleton, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());

    Skeleton *skeleton() const;
    void setSkeleton(Skeleton *skeleton);

private:
    Skeleton *m_skeleton;
};
}

#endif // BONESENSOR_H
