#include "geometry.h"

Geometry::Ray::Ray(glm::vec3 p, glm::vec3 d)
    : p(p)
    , d(d)
{}

Geometry::Ray Geometry::Ray::transform(glm::mat4 t) const
{
    return Ray(glm::vec3(t * glm::vec4(p, 1)), glm::vec3(t * glm::vec4(d, 0)));
}

glm::vec3 Geometry::Ray::solve(float t) const
{
    return p + t * d;
}


Geometry::Plane::Plane(glm::vec3 p, glm::vec3 n)
    : p(p)
    , n(n)
{}

float Geometry::Plane::intersect(Geometry::Ray r)
{
    return glm::dot(p - r.p,  n) / glm::dot(r.d, n);
}


Geometry::Camera::Camera(float near, float far, float fov, QOpenGLWindow *m_window)
    : near(near)
    , far(far)
    , fov(fov)
    , m_window(m_window)
{}

Geometry::Ray Geometry::Camera::computeRay(float pixelX, float pixelY)
{
    float width = (float) (m_window->size().width()), height = (float) (m_window->size().height());
    glm::vec2 normalizedPixelPos = glm::vec2(pixelX / width, (pixelY / height) * (height / width));
    float h = normalizedPixelPos.y /2;
    float theta = glm::radians(fov / 2);
    float d = h / glm::tan(theta);

    return Ray(glm::vec3(0), glm::normalize(glm::vec3(normalizedPixelPos, d)));

}


