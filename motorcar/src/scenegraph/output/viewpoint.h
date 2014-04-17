#ifndef GLCAMERANODE_H
#define GLCAMERANODE_H
#include "../../geometry.h"
#include "../virtualnode.h"
struct wl_global;

#include "wayland-server.h"
#include "wayland-server-protocol.h"
#include "motorcar-server-protocol.h"
//#include <GL/gl.h>
//#include "display.h"



namespace motorcar {
class Display;
class ViewPoint : public VirtualNode
{
public:
    //centerOfProjection: center of projection in camera space, applied as a translation to the projection matrix
    ViewPoint(float near, float far, Display *display, SceneGraphNode *parent,
              glm::mat4 transform = glm::mat4(), glm::vec4 viewPortParams = glm::vec4(0,0,1,1), glm::vec3 centerOfProjection = glm::vec3(0));
    ~ViewPoint();

    void updateViewMatrix();
    void updateProjectionMatrix();
    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;


    //returns camera vertical field of view in degrees
    float fov();


    //normalized viewport within a given window,
    //all numerical arguments range from 0 to 1 and are multiplied by the size of the window in all getters and before being passed to OpenGL calls
    class ViewPort{
    public:

        ViewPort(float offsetX, float offsetY, float width, float height, Display *display);

        float offsetX() const;
        float offsetY() const;
        float width() const;
        float height() const;

        //calls glViewport with parameters
        void set() const;


        glm::vec2 displayCoordsToViewportCoords(float pixelX, float pixelY) const;

        //takes a buffer of at least 8 floats and fills it with the UV space texture coordinates for the four corners of the viewport
        void uvCoords(float *buf);

        //returns a vector of the four floats which this was constructed on
        glm::vec4 viewportParams() const;

        Display *display() const;

    private:
        float m_offsetX, m_offsetY, m_width, m_height;
        Display *m_display;
    };

    Geometry::Ray worldRayAtDisplayPosition(float pixelX, float pixelY);

    ViewPort *viewport() const;
    void setViewport(ViewPort *viewport);

    glm::vec4 centerOfFocus() const;


    motorcar_viewpoint *viewpointHandle() const;
    void setViewpointHandle(motorcar_viewpoint *viewpointHandle);

    void sendViewMatrixToClients();
    void sendProjectionMatrixToClients();
    void sendViewPortToClients();
    void sendCurrentStateToSingleClient(wl_resource *resource);

    wl_global *global() const;
    void setGlobal(wl_global *global);

    ViewPort *clientColorViewport() const;
    void setClientColorViewport(ViewPort *clientColorViewport);

    ViewPort *clientDepthViewport() const;
    void setClientDepthViewport(ViewPort *clientDepthViewport);

private:
    float near, far;
    ViewPort *m_viewport;
    ViewPort *m_clientColorViewport;
    ViewPort *m_clientDepthViewport;


    //center of projection information
    glm::vec4 m_centerOfFocus;
    glm::mat4 m_COFTransform;


    //cached matrices
    glm::mat4 m_viewMatrix, m_projectionMatrix, m_viewProjectionMatrix;

    struct motorcar_viewpoint *m_viewpointHandle;
    struct wl_global *m_global;
    std::vector<struct wl_resource*> m_resources;

    static void destroy_func(struct wl_resource *resource);

    static void bind_func(struct wl_client *client, void *data,
                          uint32_t version, uint32_t id);

    struct wl_array m_viewArray, m_projectionArray;



};
}



#endif // GLCAMERANODE_H
