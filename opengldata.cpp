#include "opengldata.h"


OpenGLData::OpenGLData(QOpenGLWindow *window, SceneGraphNode *camera)
    : m_window(window)
    , m_textureBlitter(0)
    , m_surfaceShader(new QOpenGLShaderProgram)
    , m_cameraNode(camera)
    , m_camera(new Geometry::Camera(0.01, 100, 45, m_window))
    , m_ppcm(64)
    , m_viewMatrix(glm::mat4(1))
{
    m_window->makeCurrent();

    m_textureCache = new QOpenGLTextureCache(m_window->context());
    m_textureBlitter = new TextureBlitter();
    m_backgroundImage = makeBackgroundImage(QLatin1String(":/background.jpg"));

    QOpenGLFunctions *functions = m_window->context()->functions();
    functions->glGenFramebuffers(1, &m_surface_fbo);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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
    m_projectionMatrix = glm::perspective(m_camera->fov, ((float) m_window->size().width())/ ((float) m_window->size().height()), m_camera->near, m_camera->far);
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

glm::mat4 OpenGLData::viewProjectionMatrix() const
{
    return m_viewProjectionMatrix;
}

float OpenGLData::ppcm()
{
    return m_ppcm;
}

glm::mat4 OpenGLData::projectionMatrix() const
{
    return m_projectionMatrix;
}


glm::mat4 OpenGLData::viewMatrix() const
{
    return m_viewMatrix;
}


