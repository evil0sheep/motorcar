#include "defaultdisplaynode.h"

DefaultDisplayNode::DefaultDisplayNode(QObject *parent, glm::mat4 transform, OpenGLData *glData)
    :DisplayNode(parent, transform)
    , m_glInfo(glData)
    , m_cameraNode(NULL)
{
    m_cameraNode = new GLCameraNode(this, glm::mat4(1), .01, 100, 45);
    m_cameraNode->setViewport(this->computeViewport());
}

bool DefaultDisplayNode::drawSurfaceNode(QtwaylandSurfaceNode *node)
{
    GLuint texture = node->composeSurface(node->surface(), glInfo());
    //QRect geo(m_surface->pos().toPoint(),m_surface->size());
    //glData->m_textureBlitter->drawTexture(texture,geo,glData->m_window->size(),0,false,m_surface->isYInverted());


    const GLfloat textureCoordinates[] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0
    };
    const GLfloat vertexCoordinates[] ={
        0, 0, 0,
        0, 1, 0,
        1, 1, 0,
        1, 0, 0
    };

    glInfo()->m_surfaceShader->bind();
    //glViewport(0,0,glInfo()->m_window->size().width(),glInfo()->m_window->size().height());

    m_cameraNode->viewport().set();

    GLint aPositionLocation =  glInfo()->m_surfaceShader->attributeLocation("aPosition");
    GLint aTexCoordLocation =  glInfo()->m_surfaceShader->attributeLocation("aTexCoord");
    GLint uMVPMatLocation = glInfo()->m_surfaceShader->uniformLocation("uMVPMatrix");
//            GLint uModelMatrix = glData->m_surfaceShader->uniformLocation("uModelMatrix");
//            GLint uViewMatrix = glData->m_surfaceShader->uniformLocation("uViewMatrix");
//            GLint uProjectionMatrix = glData->m_surfaceShader->uniformLocation("uProjectionMatrix");

    if(aPositionLocation < 0 || aTexCoordLocation < 0 || uMVPMatLocation< 0 ){//|| uModelMatrix < 0 || uViewMatrix < 0 || uProjectionMatrix < 0){
        qDebug() << "problem with surface shader handles: " << aPositionLocation << ", "<< aTexCoordLocation << ", " << uMVPMatLocation ;// << ", " << uModelMatrix << ", "  << uViewMatrix << ", "  << uProjectionMatrix ;
    }

    node->computeSurfaceTransform(glInfo()->ppcm());
    QMatrix4x4 MVPMatrix(glm::value_ptr(glm::transpose( m_cameraNode->projectionMatrix() * m_cameraNode->viewMatrix() *  node->worldTransform() * node->surfaceTransform())));
//            QMatrix4x4 modelMatrix(glm::value_ptr(this->worldTransform()));
//            QMatrix4x4 viewMatrix(glm::value_ptr(glData->viewMatrix()));
//            QMatrix4x4 projectionMatrix(glm::value_ptr(glData->project.ionMatrix()));

    QOpenGLContext *currentContext = QOpenGLContext::currentContext();
    currentContext->functions()->glEnableVertexAttribArray(aPositionLocation);
    currentContext->functions()->glEnableVertexAttribArray(aTexCoordLocation);

    currentContext->functions()->glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 0, vertexCoordinates);
    currentContext->functions()->glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, textureCoordinates);
    glInfo()->m_surfaceShader->setUniformValue(uMVPMatLocation, MVPMatrix);
//            glData->m_surfaceShader->setUniformValue(uModelMatrix, modelMatrix);
//            glData->m_surfaceShader->setUniformValue(uViewMatrix, viewMatrix);
//            glData->m_surfaceShader->setUniformValue(uProjectionMatrix, projectionMatrix);

    //Geometry::printMatrix(glData->projectionMatrix() * (glData->viewMatrix() *  this->worldTransform() * this->surfaceTransform()));
//            Geometry::printMatrix( this->surfaceTransform());


    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);

    currentContext->functions()->glDisableVertexAttribArray(aPositionLocation);
    currentContext->functions()->glDisableVertexAttribArray(aTexCoordLocation);


    glInfo()->m_surfaceShader->release();
    return true;
}

bool DefaultDisplayNode::drawSceneGraph(float dt, SceneGraphNode *sceneGraphRoot)
{
    glInfo()->m_window->makeCurrent();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_cameraNode->setViewport(this->computeViewport());
    m_cameraNode->calculateVPMatrix();
    return DisplayNode::drawSceneGraph(dt, sceneGraphRoot);
}

Geometry::Ray DefaultDisplayNode::worldrayAtDisplayPosition(float pixelX, float pixelY)
{
    return m_cameraNode->worldrayAtBufferPosition(pixelX, pixelY);
}


OpenGLData *DefaultDisplayNode::glInfo() const
{
    return m_glInfo;
}

void DefaultDisplayNode::setGlInfo(OpenGLData *glInfo)
{
    m_glInfo = glInfo;
}

GLCameraNode::GLViewPort DefaultDisplayNode::computeViewport()
{
    return GLCameraNode::GLViewPort(0, 0, glInfo()->m_window->size().width(),  glInfo()->m_window->size().height());
}
