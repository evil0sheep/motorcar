#ifndef SCENE_H
#define SCENE_H

#include "physicalnode.h"
#include "output/display/display.h"

namespace motorcar {
class Display;
class Scene : public PhysicalNode
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
