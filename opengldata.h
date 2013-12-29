#ifndef OPENGLDATA_H
#define OPENGLDATA_H

#include "textureblitter.h"
#include "qopenglwindow.h"
#include <QtGui/private/qopengltexturecache_p.h>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QLinkedList>

class OpenGLData
{
public:
    QOpenGLWindow *m_window;
    QImage m_backgroundImage;
    GLuint m_backgroundTexture;
    TextureBlitter *m_textureBlitter;
    QOpenGLTextureCache *m_textureCache;
    GLuint m_surface_fbo;

    OpenGLData(QOpenGLWindow *window);
    ~OpenGLData();
    QImage makeBackgroundImage(const QString &fileName);
};

#endif // OPENGLDATA_H
