#include "defaultdisplaynode.h"

using namespace motorcar;
using namespace qtmotorcar;

DefaultDisplayNode::DefaultDisplayNode(Scene *scene, OpenGLData *glInfo)
    :Display()
    ,m_glInfo(glInfo)

{


}

void DefaultDisplayNode::renderSurfaceNode(WaylandSurfaceNode *surfaceNode, GLCamera *camera)
{

    GLuint texture = surfaceNode->surface()->texture();
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

    camera->viewport()->set();

    GLint aPositionLocation =  glInfo()->m_surfaceShader->attributeLocation("aPosition");
    GLint aTexCoordLocation =  glInfo()->m_surfaceShader->attributeLocation("aTexCoord");
    GLint uMVPMatLocation = glInfo()->m_surfaceShader->uniformLocation("uMVPMatrix");
//            GLint uModelMatrix = glData->m_surfaceShader->uniformLocation("uModelMatrix");
//            GLint uViewMatrix = glData->m_surfaceShader->uniformLocation("uViewMatrix");
//            GLint uProjectionMatrix = glData->m_surfaceShader->uniformLocation("uProjectionMatrix");

    if(aPositionLocation < 0 || aTexCoordLocation < 0 || uMVPMatLocation< 0 ){//|| uModelMatrix < 0 || uViewMatrix < 0 || uProjectionMatrix < 0){
        qDebug() << "problem with surface shader handles: " << aPositionLocation << ", "<< aTexCoordLocation << ", " << uMVPMatLocation ;// << ", " << uModelMatrix << ", "  << uViewMatrix << ", "  << uProjectionMatrix ;
    }

    surfaceNode->computeSurfaceTransform(glInfo()->ppcm());
    QMatrix4x4 MVPMatrix(glm::value_ptr(glm::transpose( camera->projectionMatrix() * camera->viewMatrix() *  surfaceNode->worldTransform() * surfaceNode->surfaceTransform())));
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

}

void DefaultDisplayNode::prepare()
{
    Display::prepare();
    glInfo()->m_window->makeCurrent();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}



Geometry::Ray DefaultDisplayNode::worldRayAtDisplayPosition(float pixelX, float pixelY)
{
    return viewpoints().front()->worldRayAtDisplayPosition(pixelX, pixelY);
}

glm::ivec2 DefaultDisplayNode::size()
{
    return glm::ivec2(m_glInfo->m_window->size().width(), m_glInfo->m_window->size().height());
}


OpenGLData *DefaultDisplayNode::glInfo() const
{
    return m_glInfo;
}




