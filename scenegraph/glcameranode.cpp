#include "glcameranode.h"



GLCameraNode::GLCameraNode(QObject *parent, glm::mat4 transform, float near, float far, float fov)
    :SceneGraphNode(parent, transform)
    , near(near)
    , far(far)
    , fov(fov)
    , m_viewport(0, 0, 0, 0)

{
}




Geometry::Ray GLCameraNode::worldrayAtBufferPosition(float pixelX, float pixelY)
{
    float width = (float) (m_viewport.width), height = (float) (m_viewport.height);
    glm::vec2 normalizedPixelPos = glm::vec2(-1.f *(pixelX / width - 0.5f), -1.f *(pixelY / height - 0.5f) * (height / width));
    float h = (height/width) /2;
    float theta = glm::radians(fov / 2);
    float d = h / glm::tan(theta);

    return Geometry::Ray(glm::vec3(0), glm::normalize(glm::vec3(normalizedPixelPos, d))).transform(this->worldTransform());

}

void GLCameraNode::calculateVPMatrix()
{
    m_projectionMatrix = glm::perspective(fov, ((float) m_viewport.width)/ ((float) m_viewport.height), near, far);

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


GLCameraNode::GLViewPort::GLViewPort(int offsetX, int offsetY, int width, int height)
    : offsetX(offsetX)
    , offsetY(offsetY)
    , width(width)
    , height(height)
{
}

void GLCameraNode::GLViewPort::set()
{
    glViewport(offsetX,offsetY,width,height);
}
