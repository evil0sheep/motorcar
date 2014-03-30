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


    WindowManager *windowManager() const;
    void setWindowManager(WindowManager *windowManager);

    Compositor *compositor() const;
    void setCompositor(Compositor *compositor);

    void addDisplay(Display *display);
    std::vector<Display *> displays() const;


    Scene *trash() const;
    void setTrash(Scene *trash);

private:
    WindowManager *m_windowManager;
    Compositor *m_compositor;
    Scene *m_trash;

    std::vector<Display *> m_displays;

};
}


#endif // SCENE_H
