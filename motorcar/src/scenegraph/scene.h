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

    WaylandSurfaceNode *cursorNode() const;
    void setCursorNode(WaylandSurfaceNode *cursorNode);

    glm::ivec2 cursorHotspot() const;
    void setCursorHotspot(const glm::ivec2 &cursorHotspot);

private:
    std::vector<Display *> m_displays;
    WaylandSurfaceNode * m_cursorNode;
    glm::ivec2 m_cursorHotspot;

};
}


#endif // SCENE_H
