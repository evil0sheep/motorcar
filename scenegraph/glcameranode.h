#ifndef GLCAMERANODE_H
#define GLCAMERANODE_H
#include "geometry.h"
#include "scenegraphnode.h"


class GLCameraNode : public SceneGraphNode
{
public:
    GLCameraNode(QObject *parent, glm::mat4 transform, float near, float far, float fov);
    Geometry::Ray worldrayAtBufferPosition(float pixelX, float pixelY);

    void calculateVPMatrix();
    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;
    glm::mat4 viewProjectionMatrix() const;

    struct GLViewPort{
        int offsetX, offsetY, width, height;
        GLViewPort(int offsetX,int offsetY,int width,int height);
        void set();
    };

    GLViewPort viewport() const;
    void setViewport(const GLViewPort &viewport);

private:
    float near, far, fov;
    GLViewPort m_viewport;
    glm::mat4 m_viewMatrix, m_projectionMatrix, m_viewProjectionMatrix;

};


#endif // GLCAMERANODE_H
