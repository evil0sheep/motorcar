#ifndef SPATIALMOUSE_H
#define SPATIALMOUSE_H

#include <glm/gtc/type_ptr.hpp>


#include "../physicalnode.h"
#include "../output/wireframenode.h"
#include "../output/wayland/waylandsurfacenode.h"
#include "../output/wayland/depthcompositedsurfacenode.h"
#include "../../wayland/input/seat.h"
#include "../../wayland/input/pointer.h"


#include "wayland-server.h"
#include "wayland-server-protocol.h"
#include "motorcar-server-protocol.h"

namespace motorcar {
class SixDOFPointingDevice : public PhysicalNode
{
public:
    SixDOFPointingDevice(Seat *seat, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~SixDOFPointingDevice() {}


    ///updates the current device state and moves any attaches surfaces
    virtual void handleFrameBegin(Scene *scene) override;

    bool leftMouseDown() const;
    void setLeftMouseDown(bool leftMouseDown);

    bool rightMouseDown() const;
    void setRightMouseDown(bool rightMouseDown);

    bool middleMouseDown() const;
    void setMiddleMouseDown(bool middleMouseDown);



    void grabSurfaceUnderCursor();
    void releaseGrabbedSurface();



    Seat *seat() const;
    void setSeat(Seat *seat);

    struct wl_resource *resourceForClient(struct wl_client *client);

private:

    void mouseEvent(MouseEvent::Event event, MouseEvent::Button button);
    void sixDofPointerEvent(MotorcarSurfaceNode *surfaceNode, SixDofEvent event);

    Geometry::RaySurfaceIntersection *m_latestIntersection;
    bool m_leftMouseDown, m_rightMouseDown, m_middleMouseDown;

    WaylandSurfaceNode *m_grabbedSurfaceNode;
    glm::mat4 m_grabbedSurfaceNodeTransform;

    Seat *m_seat;


    struct motorcar_3d_pointer *m_pointerHandle;
    struct wl_global *m_global;
    std::map<struct wl_client *, struct wl_resource *> m_resourcesMap;


    static void destroy_func(struct wl_resource *resource);

    static void bind_func(struct wl_client *client, void *data,
                          uint32_t version, uint32_t id);

    struct wl_array m_positionArray, m_orientationArray;

    MotorcarSurfaceNode *m_sixDofFocus;

};
}


#endif // SPATIALMOUSE_H
