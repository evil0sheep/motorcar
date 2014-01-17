#include "glcameranode.h"



GLCameraNode::GLCameraNode(QObject *parent, glm::mat4 transform, float near, float far, float fov, QWindow *window)
    :SceneGraphNode(parent, transform)
    , near(near)
    , far(far)
    , fov(fov)
    , m_viewport(0, 0, 1, 1, window)

{
}


Geometry::Ray GLCameraNode::worldRayAtDisplayPosition(float pixelX, float pixelY)
{


    glm::vec2 normalizedPixelPos = -1.f * m_viewport.displayCoordsToViewportCoords(pixelX, pixelY);
    float h = (m_viewport.height()/m_viewport.width()) /2;
    float theta = glm::radians(fov / 2);
    float d = h / glm::tan(theta);

    return Geometry::Ray(glm::vec3(0), glm::normalize(glm::vec3(normalizedPixelPos, d))).transform(this->worldTransform());

}

void GLCameraNode::calculateVPMatrix()
{
    m_projectionMatrix = glm::perspective(fov, (m_viewport.width())/ (m_viewport.height()), near, far);

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

glm::mat4 GLCameraNode::viewProjectionMatrix() const
{
    return m_viewProjectionMatrix;
}


glm::mat4 GLCameraNode::projectionMatrix() const
{
    return m_projectionMatrix;
}


glm::mat4 GLCameraNode::viewMatrix() const
{
    return m_viewMatrix;
}

GLCameraNode::GLViewPort GLCameraNode::viewport() const
{
    return m_viewport;
}

void GLCameraNode::setViewport(const GLViewPort &viewport)
{
    m_viewport = viewport;
}


GLCameraNode::GLViewPort::GLViewPort(float offsetX,float offsetY,float width,float height, QWindow *window)
    : m_offsetX(offsetX)
    , m_offsetY(offsetY)
    , m_width(width)
    , m_height(height)
    , m_window(window)
{
}

void GLCameraNode::GLViewPort::set() const
{
    glViewport(offsetX(), offsetY(), width(), height());
}

glm::vec2 GLCameraNode::GLViewPort::displayCoordsToViewportCoords(float pixelX, float pixelY) const
{
    return glm::vec2(((pixelX - offsetX()) / width() - 0.5f), ((pixelY - offsetY()) / height()  - 0.5f) * (height() / width()));
}


float GLCameraNode::GLViewPort::height() const
{
    return m_height * m_window->size().height();
}

float GLCameraNode::GLViewPort::width() const
{
    return m_width * m_window->size().width();
}

float GLCameraNode::GLViewPort::offsetY() const
{
    return m_offsetY * m_window->size().height();
}

float GLCameraNode::GLViewPort::offsetX() const
{
    return m_offsetX * m_window->size().width();
}




