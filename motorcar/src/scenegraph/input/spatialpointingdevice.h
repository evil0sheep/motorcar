#ifndef SPATIALMOUSE_H
#define SPATIALMOUSE_H

#include "inputelement.h"
#include "../physicalnode.h"

namespace motorcar {
class SpatialPointingDevice : public InputElement, public PhysicalNode
{
public:
    SpatialPointingDevice(PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~SpatialPointingDevice() {}
};
}


#endif // SPATIALMOUSE_H
