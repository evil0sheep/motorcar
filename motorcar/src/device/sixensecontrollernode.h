#ifndef SIXENSECONTROLLERNODE_H
#define SIXENSECONTROLLERNODE_H
#include "../scenegraph/input/spatialpointingdevice.h"
#include <sixense.h>

namespace motorcar {
class SixenseControllerNode : public SpatialPointingDevice
{
public:
    SixenseControllerNode(int controllerIndex, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());

    int controllerIndex() const;
    void setControllerIndex(int controllerIndex);

    void traverseNode(Scene *scene, long deltaMillis) override;

    void printStatus();

private:
    int m_controllerIndex;
};
}


#endif // SIXENSECONTROLLERNODE_H
