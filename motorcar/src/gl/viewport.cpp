#include "viewport.h"

using namespace motorcar;

ViewPort::ViewPort(glm::vec2 normalizedPos, glm::vec2 normalizedSize, Geometry::Rectangle *bufferGeometry)
    : m_offsetX(normalizedPos.x)
    , m_offsetY(normalizedPos.y)
    , m_width(normalizedSize.x)
    , m_height(normalizedSize.y)
    , m_bufferGeometry(bufferGeometry)
{

}

void ViewPort::set() const
{
    glViewport(offsetX(), offsetY(), width(), height());
}

glm::vec2 ViewPort::displayCoordsToViewportCoords(float pixelX, float pixelY) const
{
    return glm::vec2(((pixelX - offsetX()) / width() - 0.5f), ((pixelY - offsetY()) / height()  - 0.5f) * (height() / width()));
}

void ViewPort::uvCoords(float *buf)
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


float ViewPort::height() const
{
    return m_height * m_bufferGeometry->size().y;
}



float ViewPort::width() const
{
    return m_width * m_bufferGeometry->size().x;
}

float ViewPort::offsetY() const
{
    return m_offsetY * m_bufferGeometry->size().y;
}





float ViewPort::offsetX() const
{
    return m_offsetX * m_bufferGeometry->size().x;
}


glm::ivec2 ViewPort::size()
{
    return glm::ivec2(width(), height());
}
