#include "opengldata.h"


OpenGLData::OpenGLData(QOpenGLWindow *window, SceneGraphNode *camera)
    : m_window(window)
    , m_textureBlitter(0)
    , m_surfaceShader(new QOpenGLShaderProgram)
    , m_cameraNode(camera)
    , m_fov(45.f)
    , m_ppcm(16)
{
    m_window->makeCurrent();

    m_textureCache = new QOpenGLTextureCache(m_window->context());
    m_textureBlitter = new TextureBlitter();
    m_backgroundImage = makeBackgroundImage(QLatin1String(":/background.jpg"));

    QOpenGLFunctions *functions = m_window->context()->functions();
    functions->glGenFramebuffers(1, &m_surface_fbo);

    glEnable(GL_BLEND);
    glBlendFunc (GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

    m_surfaceShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "motorcarsurface.vert");
    m_surfaceShader->addShaderFromSourceFile(QOpenGLShader::Fragment,"motorcarsurface.frag");
    m_surfaceShader->link();
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

void OpenGLData::calculateVPMatrix()
{
    m_projectionMatrix = glm::perspective(m_fov, ((float) m_window->size().width())/ ((float) m_window->size().height()), 0.01f, 100.f);
    if(m_cameraNode != NULL){
        glm::mat4 trans = m_cameraNode->worldTransform();
        glm::vec3 center = glm::vec3(trans * glm::vec4(0, 0, 0, 1));
        glm::vec3 target = glm::vec3(trans * glm::vec4(0, 0, 1, 1));
        glm::vec3 up = glm::normalize(glm::vec3(trans * glm::vec4(0, 1, 0, 0)));
//        glm::vec3 vec = target;
//        qDebug() << vec.x << ", " << vec.y << ", " << vec.z;
        m_viewMatrix = glm::lookAt(center, target, up);
    }
   //m_viewMatrix = glm::lookAt(glm::vec3(0,0,-1.f), glm::vec3(0,0,0), glm::vec3(0,1,0));
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

glm::mat4 OpenGLData::viewProjectionMatrix()
{
    return m_viewProjectionMatrix;
}

float OpenGLData::ppcm()
{
    return m_ppcm;
}
