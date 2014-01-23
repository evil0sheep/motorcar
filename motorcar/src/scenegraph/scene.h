#ifndef SCENE_H
#define SCENE_H
#include "scenegraphnode.h"

namespace motorcar {
class Scene : public SceneGraphNode
{
public:
    Scene();
    virtual ~Scene();
    //returns whether or not the given SceneGraphNode exists in the SceneGraph subtree rooted at this node
    bool containsNode(SceneGraphNode *node);

};
}


#endif // SCENE_H
