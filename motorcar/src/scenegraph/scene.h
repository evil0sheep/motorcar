#ifndef SCENE_H
#define SCENE_H

#include "scenegraphnode.h"
#include "output/display.h"
namespace motorcar {
class Scene : public SceneGraphNode
{
public:
    Scene();
    virtual ~Scene();

    //notifies this scene that a node has been added to the scene
    virtual void notifyNodeAdded(SceneGraphNode *node);

private:
    std::vector<Display> m_displays;
};
}


#endif // SCENE_H
