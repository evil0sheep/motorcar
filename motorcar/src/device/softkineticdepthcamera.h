#ifndef SOFTKINETICDEPTHCAMERA_H
#define SOFTKINETICDEPTHCAMERA_H
#include <DepthSense.hxx>
#include <stdio.h>
#include <vector>
#include <exception>
#include <thread>
#include "../scenegraph/output/drawable.h"
#include "../gl/openglshader.h"

namespace motorcar {
class SoftKineticDepthCamera : public Drawable
{
public:
    SoftKineticDepthCamera(SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4());
    ~SoftKineticDepthCamera();

    virtual void draw(Scene *scene, Display *display) override;

private:
    OpenGLShader *m_pointCloudShader;
    std::thread m_cameraThread;

    GLint h_aPosition, h_aConfidence,h_aTexCoord, h_uMVPMatrix;
    GLuint m_colorTexture;
    GLuint m_indexBuffer;


    const int m_colorWidth = 640, m_colorHeight = 480;

};
}


#endif // SOFTKINETICDEPTHCAMERA_H
