#ifndef SIXENSEBASENODE_H
#define SIXENSEBASENODE_H

#include "../scenegraph/physicalnode.h"
#include "sixensecontrollernode.h"
#include "../scenegraph/scene.h"

namespace motorcar {
class SixenseBaseNode : public PhysicalNode
{
public:
    SixenseBaseNode(int baseIndex, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());

    ///gets current system state and passes new controller state to controller nodes
    virtual void handleFrameBegin(Scene *scene) override;



    bool connected() const;
    void setConnected(bool connected);

    std::vector<SixenseControllerNode *> controllers() const;

private:
    std::vector<SixenseControllerNode *> m_controllers;
    int m_baseIndex;
    bool m_connected;
};
}


#endif // SIXENSEBASENODE_H
