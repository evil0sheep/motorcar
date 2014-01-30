#include "glcameranode.h"
#include "display.h"

using namespace motorcar;

GLCamera::GLCamera(SceneGraphNode &parent, glm::mat4 transform, float near, float far, float fov, Display *display)
    :VirtualNode(parent, transform)
    , near(near)
    , far(far)
    , fov(fov)
    , m_viewport(new GLViewPort(0, 0, 1, 1, display))

{
}


Geometry::Ray GLCamera::worldRayAtDisplayPosition(float pixelX, float pixelY)
{


    glm::vec2 normalizedPixelPos = -1.f * m_viewport->displayCoordsToViewportCoords(pixelX, pixelY);
    float h = (m_viewport->height()/m_viewport->width()) /2;
    float theta = glm::radians(fov / 2);
    float d = h / glm::tan(theta);

    return Geometry::Ray(glm::vec3(0), glm::normalize(glm::vec3(normalizedPixelPos, d))).transform(this->worldTransform());

}

void GLCamera::calculateVPMatrix()
{
    m_projectionMatrix = glm::perspective(fov, (m_viewport->width())/ (m_viewport->height()), near, far);

    glm::mat4 trans = worldTransform();
    glm::vec3 center = glm::vec3(trans * glm::vec4(0, 0, 0, 1));
    glm::vec3 target = glm::vec3(trans * glm::vec4(0, 0, 1, 1));
    glm::vec3 up = glm::normalize(glm::vec3(trans * glm::vec4(0, 1, 0, 0)));
    //        glm::vec3 vec = target;
    //        qDebug() << vec.x << ", " << vec.y << ", " << vec.z;
    m_viewMatrix = glm::lookAt(center, target, up);

    //m_viewMatrix = glm::lookAt(glm::vec3(0,0,-1.f), glm::vec3(0,0,0), glm::vec3(0,1,0));
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

glm::mat4 GLCamera::viewProjectionMatrix() const
{
    return m_viewProjectionMatrix;
}


glm::mat4 GLCamera::projectionMatrix() const
{
    return m_projectionMatrix;
}


glm::mat4 GLCamera::viewMatrix() const
{
    return m_viewMatrix;
}


GLCamera::GLViewPort *GLCamera::viewport() const
{
    return m_viewport;
}

void GLCamera::setViewport(GLViewPort *viewport)
{
    m_viewport = viewport;
}



GLCamera::GLViewPort::GLViewPort(float offsetX, float offsetY, float width, float height, Display *display)
    : m_offsetX(offsetX)
    , m_offsetY(offsetY)
    , m_width(width)
    , m_height(height)
    , m_display(display)
{
}

void GLCamera::GLViewPort::set() const
{
    glViewport(offsetX(), offsetY(), width(), height());
}

glm::vec2 GLCamera::GLViewPort::displayCoordsToViewportCoords(float pixelX, float pixelY) const
{
    return glm::vec2(((pixelX - offsetX()) / width() - 0.5f), ((pixelY - offsetY()) / height()  - 0.5f) * (height() / width()));
}


float GLCamera::GLViewPort::height() const
{
    return m_height * m_display->size().y;
}

float GLCamera::GLViewPort::width() const
{
    return m_width * m_display->size().x;
}

float GLCamera::GLViewPort::offsetY() const
{
    return m_offsetY * m_display->size().y;
}

float GLCamera::GLViewPort::offsetX() const
{
    return m_offsetX * m_display->size().x;
}







Display *GLCamera::GLViewPort::display() const
{
    return m_display;
}

void GLCamera::GLViewPort::setDisplay(Display *display)
{
    m_display = display;
}
