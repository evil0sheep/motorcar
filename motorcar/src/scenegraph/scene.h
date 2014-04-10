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

    void draw(long deltaMillis);

    ///Overloads SceneGraphNode definition to return this node
    Scene *scene() override;


    WindowManager *windowManager() const;
    void setWindowManager(WindowManager *windowManager);

    Compositor *compositor() const;
    void setCompositor(Compositor *compositor);

    void addDisplay(Display *display);
    std::vector<Display *> displays() const;



private:

    WindowManager *m_windowManager;
    Compositor *m_compositor;
    Scene *m_trash;

    std::vector<Display *> m_displays;

};
}


#endif // SCENE_H
