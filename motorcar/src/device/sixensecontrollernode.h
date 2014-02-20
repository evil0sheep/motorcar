#ifndef SIXENSECONTROLLERNODE_H
#define SIXENSECONTROLLERNODE_H
#include "../scenegraph/input/spatialpointingdevice.h"
#include <sixense.h>

namespace motorcar {
class SixenseControllerNode : public SpatialPointingDevice
{
public:
    SixenseControllerNode(int controllerIndex, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());

    void updateState(sixenseControllerData data);

    int controllerIndex() const;
    void setControllerIndex(int controllerIndex);

    void printStatus();

    bool enabled() const;
    void setEnabled(bool enabled);

    //inhereted from SceneGraphNode
    void traverseNode(Scene *scene, long deltaMillis) override;

private:
    int m_controllerIndex;
    bool m_enabled;
};
}


#endif // SIXENSECONTROLLERNODE_H
