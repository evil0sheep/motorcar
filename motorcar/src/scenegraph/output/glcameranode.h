#ifndef GLCAMERANODE_H
#define GLCAMERANODE_H
#include "../../geometry.h"
#include "../virtualnode.h"
#include "outputelement.h"
#include "wayland-server.h"
#include "wayland-server-protocol.h"
#include "motorcar-server-protocol.h"
//#include <GL/gl.h>
//#include "display.h"



namespace motorcar {
class Display;
class GLCamera : public OutputElement, public VirtualNode
{
public:
    //centerOfProjection: center of projection in camera space, applied as a translation to the projection matrix
    GLCamera(float near, float far, Display *display, SceneGraphNode *parent,
              glm::mat4 transform = glm::mat4(), glm::vec4 viewPortParams = glm::vec4(0,0,1,1), glm::vec3 centerOfProjection = glm::vec3(0));
    ~GLCamera();

    void calculateVPMatrix();
    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;
    glm::mat4 viewProjectionMatrix() const;

    //returns camera vertical field of view in degrees
    float fov();


    //normalized viewport within a given window,
    //all numerical arguments range from 0 to 1 and are multiplied by the size of the window in all getters and before being passed to OpenGL calls
    class GLViewPort{
    public:

        GLViewPort(float offsetX, float offsetY, float width, float height, Display *display);

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
        void setDisplay(Display *display);

    private:
        float m_offsetX, m_offsetY, m_width, m_height;
        Display *m_display;
    };

    Geometry::Ray worldRayAtDisplayPosition(float pixelX, float pixelY);

    GLViewPort *viewport() const;
    void setViewport(GLViewPort *viewport);

    glm::vec4 centerOfFocus() const;


    motorcar_viewpoint *viewpointHandle() const;
    void setViewpointHandle(motorcar_viewpoint *viewpointHandle);

    void sendCurrentViewpointToClients();

private:
    float near, far;
    GLViewPort *m_viewport;

    //center of projection information
    glm::vec4 m_centerOfFocus;
    glm::mat4 m_COFTransform;


    //cached matrices
    glm::mat4 m_viewMatrix, m_projectionMatrix, m_viewProjectionMatrix;

    struct motorcar_viewpoint *m_viewpointHandle;
    struct wl_global *m_global;

    static void bind_func(struct wl_client *client, void *data,
                          uint32_t version, uint32_t id);



};
}



#endif // GLCAMERANODE_H
