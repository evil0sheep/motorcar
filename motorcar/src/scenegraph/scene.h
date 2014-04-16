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




    ///Overloads SceneGraphNode definition to return this node
    Scene *scene() override;

    void prepareForFrame(long timeStampMillis);
    void drawFrame();
    void finishFrame();


    WindowManager *windowManager() const;
    void setWindowManager(WindowManager *windowManager);

    Compositor *compositor() const;
    void setCompositor(Compositor *compositor);

    void addDisplay(Display *display);
    std::vector<Display *> displays() const;

    Display *activeDisplay() const;
    void setActiveDisplay(Display *activeDisplay);


    long currentTimestampMillis() const;
    void setCurrentTimestampMillis(long currentTimestampMillis);

    long latestTimestampChange();



private:
    long m_currentTimestampMillis, m_lastTimestepMillis;
    WindowManager *m_windowManager;
    Compositor *m_compositor;
    Scene *m_trash;

    std::vector<Display *> m_displays;
    Display *m_activeDisplay;

};
}


#endif // SCENE_H
