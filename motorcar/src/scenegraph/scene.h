#ifndef SCENE_H
#define SCENE_H

#include "physicalnode.h"
#include "output/display/display.h"

namespace motorcar {
class WindowManager;
class Compositor;
class Scene : public PhysicalNode
{
public:
    Scene();
    virtual ~Scene();

    //notifies this scene that a node has been added to the scene
    virtual void notifyNodeAdded(SceneGraphNode *node);



    void draw(long deltaMillis);

    WaylandSurfaceNode *cursorNode() const;
    void setCursorNode(WaylandSurfaceNode *cursorNode);

    glm::ivec2 cursorHotspot() const;
    void setCursorHotspot(const glm::ivec2 &cursorHotspot);

    WindowManager *windowManager() const;
    void setWindowManager(WindowManager *windowManager);

    Compositor *compositor() const;
    void setCompositor(Compositor *compositor);

    void addDisplay(Display *display);
    std::vector<Display *> displays() const;


private:
    WindowManager *m_windowManager;
    Compositor *m_compositor;
    WaylandSurfaceNode * m_cursorNode;
    glm::ivec2 m_cursorHotspot;
    std::vector<Display *> m_displays;

};
}


#endif // SCENE_H
