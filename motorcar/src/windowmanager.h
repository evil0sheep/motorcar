#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "scenegraph/output/wayland/waylandsurfacenode.h"
#include "scenegraph/scene.h"
#include "events/events.h"
#include <map>

namespace motorcar{
///Handles input events and window positioning
/*This class is responsible for creating surfaceNodes for new surfaces, and for positioning them in the scene,
 *as well as delivering events to surfaces based on incoming events from other */
class WindowManager
{
public:


    WindowManager(Scene *scene);

    WaylandSurfaceNode *createSurface(WaylandSurface *surface);
    void destroySurface(WaylandSurface *surface);

    WaylandSurfaceNode *mapSurface(WaylandSurface *surface, WaylandSurface::SurfaceType surfaceType);
    void unmapSurface(WaylandSurface *surface);


    void sendEvent(const Event &event);

    WaylandSurfaceNode *getSurfaceNode(WaylandSurface *surface) const;

    WaylandSurfaceNode *mouseFocus() const;
    void setMouseFocus(WaylandSurfaceNode *mouseFocus);

    WaylandSurfaceNode *keyboardFocus() const;
    void setKeyboardFocus(WaylandSurfaceNode *keyboardFocus);



    Scene *scene() const;
    void setScene(Scene *scene);

private:
    std::map<WaylandSurface *, WaylandSurfaceNode *> m_surfaceMap;
    int m_numSurfacesMapped;
    WaylandSurfaceNode * m_mouseFocus, *m_keyboardFocus;
    Scene *m_scene;
};
}


#endif // WINDOWMANAGER_H
