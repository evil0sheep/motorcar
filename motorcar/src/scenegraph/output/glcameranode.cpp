#include "glcameranode.h"
#include "display/display.h"
#include "../scene.h"
#include "../../compositor.h"


using namespace motorcar;

//const static struct motorcar_viewpoint_interface motorcarViewpointInterface;

ViewPoint::ViewPoint(float near, float far, Display *display, SceneGraphNode *parent, glm::mat4 transform, glm::vec4 viewPortParams, glm::vec3 centerOfProjection)
    :VirtualNode(parent, transform)
    , near(near)
    , far(far)
    , m_centerOfFocus(centerOfProjection, 1)
    , m_COFTransform(glm::translate(glm::mat4(1), centerOfProjection))
    , m_viewport(new ViewPort(viewPortParams.x, viewPortParams.y, viewPortParams.z, viewPortParams.w, display))
    , m_viewpointHandle(NULL)
    , m_global(NULL)

{


    std::cout << "creating viewpoint global: " << this <<std::endl;





    m_global = wl_global_create(scene()->compositor()->wlDisplay(),
                     &motorcar_viewpoint_interface,
                      motorcar_viewpoint_interface.version,
                     this,
                     ViewPoint::bind_func);

    wl_array_init(&m_viewArray);
    wl_array_init(&m_projectionArray);

    wl_array_add(&m_viewArray, sizeof(glm::mat4));
    wl_array_add(&m_projectionArray, sizeof(glm::mat4));


    updateViewMatrix();
    updateProjectionMatrix();
    sendViewPortToClients();
}

ViewPoint::~ViewPoint()
{
    delete m_viewport;
    wl_array_release(&m_viewArray);
    wl_array_release(&m_projectionArray);
}

void ViewPoint::updateViewMatrix()
{
    glm::mat4 trans = worldTransform();
    glm::vec3 center = glm::vec3(trans * glm::vec4(0, 0, 0, 1));
    glm::vec3 target = glm::vec3(trans * glm::vec4(0, 0, -1, 1));
    glm::vec3 up = glm::normalize(glm::vec3(trans * glm::vec4(0, 1, 0, 0)));
    //        glm::vec3 vec = target;
    //        qDebug() << vec.x << ", " << vec.y << ", " << vec.z;
    m_viewMatrix = glm::lookAt(center, target, up);

    this->sendViewMatrixToClients();
}

void ViewPoint::updateProjectionMatrix()
{
    m_projectionMatrix = m_COFTransform * glm::perspective(fov(), (m_viewport->width())/ (m_viewport->height()), near, far);

    this->sendProjectionMatrixToClients();
}


Geometry::Ray ViewPoint::worldRayAtDisplayPosition(float pixelX, float pixelY)
{

    glm::vec2 normalizedPixelPos = glm::vec2(-1, 1) * m_viewport->displayCoordsToViewportCoords(pixelX, pixelY);
    float h = (m_viewport->height()/m_viewport->width()) /2;
    float theta = glm::radians(fov() / 2);
    float d = h / glm::tan(theta);

    return Geometry::Ray(glm::vec3(0), glm::normalize(glm::vec3(normalizedPixelPos, d))).transform(this->worldTransform());

}



float ViewPoint::fov()
{

    //take camera distance to display and project it onto display normal
    glm::mat4 displayWorldTransform = m_viewport->display()->worldTransform();
    glm::vec4 origin(0, 0, 0, 1);
    glm::vec3 cameraToDisplayVector = glm::vec3((displayWorldTransform * origin) -  (worldTransform() * origin));
    glm::vec3 displayNormal = glm::normalize(glm::vec3(displayWorldTransform * glm::vec4(0, 0, 1, 0)));
    float eyeToScreenDistance = glm::abs(glm::dot(cameraToDisplayVector, displayNormal));


    return glm::degrees(2 * atan(m_viewport->display()->size().y / (2 * eyeToScreenDistance)));
}


glm::mat4 ViewPoint::projectionMatrix() const
{
    return m_projectionMatrix;
}


glm::mat4 ViewPoint::viewMatrix() const
{
    return m_viewMatrix;
}


ViewPoint::ViewPort *ViewPoint::viewport() const
{
    return m_viewport;
}

void ViewPoint::setViewport(ViewPort *viewport)
{
    m_viewport = viewport;
    sendViewPortToClients();
    updateProjectionMatrix();
}



ViewPoint::ViewPort::ViewPort(float offsetX, float offsetY, float width, float height, Display *display)
    : m_offsetX(offsetX)
    , m_offsetY(offsetY)
    , m_width(width)
    , m_height(height)
    , m_display(display)
{
}

void ViewPoint::ViewPort::set() const
{
    glViewport(offsetX(), offsetY(), width(), height());
}

glm::vec2 ViewPoint::ViewPort::displayCoordsToViewportCoords(float pixelX, float pixelY) const
{
    return glm::vec2(((pixelX - offsetX()) / width() - 0.5f), ((pixelY - offsetY()) / height()  - 0.5f) * (height() / width()));
}

void ViewPoint::ViewPort::uvCoords(float *buf)
{
    buf[0] = m_offsetX;
    buf[1] = m_offsetY;

    buf[2] = m_offsetX;
    buf[3] = m_offsetY + m_height;

    buf[4] = m_offsetX + m_width;
    buf[5] = m_offsetY + m_height;

    buf[6] = m_offsetX + m_width;
    buf[7] = m_offsetY;


}


float ViewPoint::ViewPort::height() const
{
    return m_height * m_display->resolution().y;
}

float ViewPoint::ViewPort::width() const
{
    return m_width * m_display->resolution().x;
}

float ViewPoint::ViewPort::offsetY() const
{
    return m_offsetY * m_display->resolution().y;
}

float ViewPoint::ViewPort::offsetX() const
{
    return m_offsetX * m_display->resolution().x;
}







Display *ViewPoint::ViewPort::display() const
{
    return m_display;
}


glm::vec4 ViewPoint::centerOfFocus() const
{
    return m_centerOfFocus;
}
motorcar_viewpoint *ViewPoint::viewpointHandle() const
{
    return m_viewpointHandle;
}

void ViewPoint::setViewpointHandle(motorcar_viewpoint *viewpointHandle)
{
    m_viewpointHandle = viewpointHandle;
}

void ViewPoint::sendViewMatrixToClients()
{
    std::memcpy(m_viewArray.data, glm::value_ptr(this->viewMatrix()), m_viewArray.size);

    for(struct wl_resource *resource : m_resources){
        motorcar_viewpoint_send_view_matrix(resource, &m_viewArray);
    }
    //
}

void ViewPoint::sendProjectionMatrixToClients()
{
    std::memcpy(m_projectionArray.data, glm::value_ptr(this->projectionMatrix()), m_projectionArray.size);
    for(struct wl_resource *resource : m_resources){
        motorcar_viewpoint_send_projection_matrix(resource, &m_projectionArray);
    }

}

void ViewPoint::sendViewPortToClients()
{
    for(struct wl_resource *resource : m_resources){
        motorcar_viewpoint_send_view_port(resource, m_viewport->offsetX(), m_viewport->offsetY(), m_viewport->width(), m_viewport->height(), 0,0,0,0);
    }
}

void ViewPoint::sendCurrentStateToSingleClient(wl_resource *resource)
{
     motorcar_viewpoint_send_view_matrix(resource, &m_viewArray);
     motorcar_viewpoint_send_projection_matrix(resource, &m_projectionArray);
     motorcar_viewpoint_send_view_port(resource, m_viewport->offsetX(), m_viewport->offsetY(), m_viewport->width(), m_viewport->height(), 0,0,0,0);
}


wl_global *ViewPoint::global() const
{
    return m_global;
}

void ViewPoint::setGlobal(wl_global *global)
{
    m_global = global;
}

void ViewPoint::destroy_func(wl_resource *resource)
{
    ViewPoint *viewpoint = static_cast<ViewPoint *>(resource->data);
    //viewpoint->m_resources.
    viewpoint->m_resources.erase(std::remove(viewpoint->m_resources.begin(), viewpoint->m_resources.end(), resource), viewpoint->m_resources.end());
}



glm::vec4 ViewPoint::ViewPort::viewportParams() const
{
    return glm::vec4(m_offsetX, m_offsetY, m_width, m_height);
}



void ViewPoint::bind_func(struct wl_client *client, void *data,
                      uint32_t version, uint32_t id)
{
    ViewPoint *viewpoint = static_cast<ViewPoint *>(data);
    std::cout << "client bound viewpoint global: " << viewpoint << std::endl;
    struct wl_resource *resource = wl_resource_create(client, &motorcar_viewpoint_interface, version, id);
    wl_resource_set_implementation(resource,NULL, data, ViewPoint::destroy_func);
    viewpoint->m_resources.push_back(resource);
    viewpoint->sendCurrentStateToSingleClient(resource);
}

