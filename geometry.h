#ifndef GEOMETRY_H
#define GEOMETRY_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "qopenglwindow.h"

class Geometry
{
public:
    Geometry();

    struct Ray
    {
        Ray(glm::vec3 p, glm::vec3 d);
        Ray transform(glm::mat4 t) const;
        glm::vec3 p, d;
        glm::vec3 solve(float t) const;
    };

    struct Plane
    {
        Plane(glm::vec3 p, glm::vec3 n);
        glm::vec3 p, n;
        float intersect(Ray r);
    };

    struct Camera
    {
        float near, far, fov;
        QOpenGLWindow *m_window;
        Camera(float near, float far, float fov, QOpenGLWindow *m_window);
        Ray computeRay(float pixelX, float pixelY);

    };

};

#endif // GEOMETRY_H
