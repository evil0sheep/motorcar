#ifndef PHYSICALNODE_H
#define PHYSICALNODE_H
#include "scenegraphnode.h"

namespace motorcar {
class PhysicalNode : public SceneGraphNode
{
public:
    PhysicalNode(PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~PhysicalNode(){}
    void setParentNode(PhysicalNode *parent);

protected:
    PhysicalNode();

};
}


#endif // PHYSICALNODE_H
