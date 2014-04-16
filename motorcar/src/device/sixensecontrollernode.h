#ifndef SIXENSECONTROLLERNODE_H
#define SIXENSECONTROLLERNODE_H
#include "../scenegraph/input/spatialpointingdevice.h"
#include "../scenegraph/input/singlebonetracker.h"
#include <sixense.h>

namespace motorcar {
class SixenseControllerNode : public PhysicalNode
{
public:
    SixenseControllerNode(int controllerIndex, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());

    void updateState(sixenseControllerData data);

    int controllerIndex() const;
    void setControllerIndex(int controllerIndex);

    bool enabled() const;
    void setEnabled(bool enabled);


    SpatialPointingDevice *pointingDevice() const;
    void setPointingDevice(SpatialPointingDevice *pointingDevice);

    SingleBoneTracker *boneTracker() const;
    void setBoneTracker(SingleBoneTracker *boneTracker);

private:
    SpatialPointingDevice *m_pointingDevice;
    SingleBoneTracker *m_boneTracker;
    int m_controllerIndex;
    bool m_enabled;
    bool m_bumperDown;

    glm::vec3 m_filteredPos;
    float m_filterConstant;
};
}


#endif // SIXENSECONTROLLERNODE_H
