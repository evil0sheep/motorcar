#include "opengldata.h"

OpenGLData::OpenGLData(QOpenGLWindow *window)
    : m_window(window)
    , m_textureBlitter(0)
{
    m_window->makeCurrent();

    m_textureCache = new QOpenGLTextureCache(m_window->context());
    m_textureBlitter = new TextureBlitter();
    m_backgroundImage = makeBackgroundImage(QLatin1String(":/background.jpg"));

    QOpenGLFunctions *functions = m_window->context()->functions();
    functions->glGenFramebuffers(1, &m_surface_fbo);
}

OpenGLData::~OpenGLData()
{
    delete m_textureBlitter;
    delete m_textureCache;
}

QImage OpenGLData::makeBackgroundImage(const QString &fileName)
{
    Q_ASSERT(m_window);

    int width = m_window->width();
    int height = m_window->height();
    QImage baseImage(fileName);
    QImage patternedBackground(width, height, baseImage.format());
    QPainter painter(&patternedBackground);

    QSize imageSize = baseImage.size();
    for (int y = 0; y < height; y += imageSize.height()) {
        for (int x = 0; x < width; x += imageSize.width()) {
            painter.drawImage(x, y, baseImage);
        }
    }

    return patternedBackground;
}
