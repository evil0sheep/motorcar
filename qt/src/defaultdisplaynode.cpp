#include "defaultdisplaynode.h"

using namespace motorcar;
using namespace qtmotorcar;



DefaultDisplayNode::DefaultDisplayNode(OpenGLContext *glContext)
    :Display()
    ,m_glContext(glContext)
    ,m_vertexShaderStream("../motorcar/src/shaders/motorcarsurface.vert")
    ,m_fragmentShaderStream("../motorcar/src/shaders/motorcarsurface.frag")
    ,m_shaderProgram(new motorcar::OpenGLShader(m_vertexShaderStream, m_fragmentShaderStream))

{
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

    glGenBuffers(1, &m_textureCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordinates);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), textureCoordinates, GL_STATIC_DRAW);

    glGenBuffers(1, &m_vertexCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoordinates);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertexCoordinates, GL_STATIC_DRAW);

    h_aPosition =  glGetAttribLocation(m_shaderProgram->handle(), "aPosition");
    h_aTexCoord =  glGetAttribLocation(m_shaderProgram->handle(), "aTexCoord");
    h_uMVPMatrix  = glGetUniformLocation(m_shaderProgram->handle(), "uMVPMatrix");

    if(h_aPosition < 0 || h_aTexCoord < 0 || h_uMVPMatrix < 0 ){
        qDebug() << "problem with surface shader handles: " << h_aPosition << ", "<< h_aTexCoord << ", " << h_uMVPMatrix ;
    }


}

void DefaultDisplayNode::renderSurfaceNode(WaylandSurfaceNode *surfaceNode, GLCamera *camera)
{


    GLuint texture = surfaceNode->surface()->texture();
    camera->viewport()->set();

    glUseProgram(m_shaderProgram->handle());

    glEnableVertexAttribArray(h_aPosition);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoordinates);
    glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(h_aTexCoord);
    glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordinates);
    glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);



    glUniformMatrix4fv(h_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(camera->projectionMatrix() * camera->viewMatrix() *  surfaceNode->worldTransform() * surfaceNode->surfaceTransform()));






    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);



    glDisableVertexAttribArray(h_aPosition);
    glDisableVertexAttribArray(h_aTexCoord);

    glUseProgram(0);

}



void DefaultDisplayNode::prepare()
{
    Display::prepare();
    glContext()->makeCurrent();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}



Geometry::Ray DefaultDisplayNode::worldRayAtDisplayPosition(float pixelX, float pixelY)
{
    return viewpoints().front()->worldRayAtDisplayPosition(pixelX, pixelY);
}

glm::ivec2 DefaultDisplayNode::size()
{
    return glContext()->defaultFramebufferSize(); //glm::ivec2(m_glInfo->m_window->size().width(), m_glInfo->m_window->size().height());
}











motorcar::OpenGLContext *DefaultDisplayNode::glContext() const
{
    return m_glContext;
}

void DefaultDisplayNode::setGlContext(motorcar::OpenGLContext *glContext)
{
    m_glContext = glContext;
}
