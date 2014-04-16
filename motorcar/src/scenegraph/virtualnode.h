#ifndef VIRTUALNODE_H
#define VIRTUALNODE_H
#include "scenegraphnode.h"

namespace motorcar {
class VirtualNode : public SceneGraphNode
{
public:
    VirtualNode(SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~VirtualNode() {}

    //applies time dependent tansforms to this node, overrride in subclasses to apply animations to virtual nodes
    //does nothing unless overridded
    virtual void animate(long deltaMillis);

    virtual void handleFrameBegin(Scene *scene) override;

    void setParentNode(SceneGraphNode *parent);


};
}


#endif // VIRTUALNODE_H
