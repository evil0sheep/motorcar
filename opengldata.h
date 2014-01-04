#ifndef OPENGLDATA_H
#define OPENGLDATA_H

#include "scenegraphnode.h"
#include "textureblitter.h"
#include "qopenglwindow.h"


#include <QtGui/private/qopengltexturecache_p.h>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QLinkedList>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>

#include <glm/glm.hpp>

class SceneGraphNode;
class OpenGLData
{
public:
    QOpenGLWindow *m_window;
    QImage m_backgroundImage;
    GLuint m_backgroundTexture;
    TextureBlitter *m_textureBlitter;
    QOpenGLTextureCache *m_textureCache;
    GLuint m_surface_fbo;
    QOpenGLShaderProgram *m_surfaceShader;
    SceneGraphNode *m_cameraNode;


    OpenGLData(QOpenGLWindow *window, SceneGraphNode *camera);
    ~OpenGLData();
    QImage makeBackgroundImage(const QString &fileName);

    void calculateVPMatrix();
    glm::mat4 viewProjectionMatrix();
    float ppcm();

private:
     float m_fov, m_ppcm;
     glm::mat4 m_viewMatrix, m_projectionMatrix, m_viewProjectionMatrix;


};

#endif // OPENGLDATA_H
