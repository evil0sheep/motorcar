#ifndef SCENE_H
#define SCENE_H

#include "scenegraphnode.h"
#include "output/display.h"

namespace motorcar {
class Display;
class Scene : public SceneGraphNode
{
public:
    Scene();
    virtual ~Scene(){}

    //notifies this scene that a node has been added to the scene
    virtual void notifyNodeAdded(SceneGraphNode *node);

    void addDisplay(Display *display);
    std::vector<Display *> displays() const;

    void draw(long deltaMillis);

private:
    std::vector<Display *> m_displays;

};
}


#endif // SCENE_H
