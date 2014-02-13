#include "spatialpointingdevice.h"

using namespace motorcar;

SpatialPointingDevice::SpatialPointingDevice(PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
{ 
}
