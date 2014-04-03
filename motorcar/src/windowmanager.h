#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "scenegraph/output/wayland/waylandsurfacenode.h"
#include "scenegraph/scene.h"
#include "events/events.h"
#include "wayland/input/waylandinput.h"
#include "compositor.h"
#include "shell.h"
#include <map>

namespace motorcar{
///Handles input events and window positioning
/*This class is responsible for creating surfaceNodes for new surfaces, and for positioning them in the scene,
 *as well as delivering events to surfaces based on incoming events from other */
class WindowManager
{
public:


    WindowManager(Scene *scene, Seat *defaultSeat);

    WaylandSurfaceNode *createSurface(WaylandSurface *surface);
    void destroySurface(WaylandSurface *surface);

    WaylandSurfaceNode *mapSurface(WaylandSurface *surface, WaylandSurface::SurfaceType surfaceType);
    void unmapSurface(WaylandSurface *surface);


    void sendEvent(const Event &event);

    WaylandSurfaceNode *getSurfaceNode(WaylandSurface *surface) const;




    Scene *scene() const;
    void setScene(Scene *scene);

    Seat *defaultSeat() const;
    void setDefaultSeat(Seat *defaultSeat);

    void ensureKeyboardFocusIsValid(WaylandSurface *oldSurface);

private:

    std::map<WaylandSurface *, WaylandSurfaceNode *> m_surfaceMap;
    int m_numSurfacesMapped;
    Scene *m_scene;
    Seat *m_defaultSeat;
    Shell *m_shell;
};
}


#endif // WINDOWMANAGER_H
