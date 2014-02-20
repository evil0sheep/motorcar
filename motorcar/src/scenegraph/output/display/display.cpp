#include "display.h"

using namespace motorcar;

Display::Display(OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
    ,m_glContext(glContext)
    ,m_surfaceShader(new motorcar::OpenGLShader(std::string("../motorcar/src/shaders/motorcarsurface.vert"), std::string("../motorcar/src/shaders/motorcarsurface.frag")))
    ,m_size(displayDimensions)

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

    h_aPosition =  glGetAttribLocation(m_surfaceShader->handle(), "aPosition");
    h_aTexCoord =  glGetAttribLocation(m_surfaceShader->handle(), "aTexCoord");
    h_uMVPMatrix  = glGetUniformLocation(m_surfaceShader->handle(), "uMVPMatrix");

    if(h_aPosition < 0 || h_aTexCoord < 0 || h_uMVPMatrix < 0 ){
       std::cout << "problem with surface shader handles: " << h_aPosition << ", "<< h_aTexCoord << ", " << h_uMVPMatrix << std::endl;
    }
}

Display::~Display()

{
    delete m_surfaceShader;
}

void Display::prepareForDraw()
{
//    std::cout << "active display transform"  <<std::endl;
//    Geometry::printMatrix(worldTransform());

    for(GLCamera *viewpoint : viewpoints()){
        viewpoint->calculateVPMatrix();
    }
    glContext()->makeCurrent();
    glClearColor(.7f, .85f, 1.f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::renderDrawable(Drawable *drawable)
{
    for(GLCamera *viewpoint : m_viewpoints){
        drawable->drawViewpoint(viewpoint);
    }
}

void Display::addViewpoint(GLCamera *v)
{
    m_viewpoints.push_back(v);
}


std::vector<GLCamera *> Display::viewpoints() const
{
    return m_viewpoints;
}

glm::vec2 Display::size() const
{
    return m_size;
}


void Display::renderSurfaceNode(WaylandSurfaceNode *surfaceNode, GLCamera *camera)
{
    camera->viewport()->set();

    GLuint texture = surfaceNode->surface()->texture();

    glUseProgram(m_surfaceShader->handle());

    glEnableVertexAttribArray(h_aPosition);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoordinates);
    glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(h_aTexCoord);
    glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordinates);
    glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glUniformMatrix4fv(h_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(camera->projectionMatrix() * camera->viewMatrix() *  surfaceNode->worldTransform() * surfaceNode->surfaceTransform()));

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(h_aPosition);
    glDisableVertexAttribArray(h_aTexCoord);

    glUseProgram(0);

}



Geometry::Ray Display::worldRayAtDisplayPosition(glm::vec2 pixel)
{
    GLCamera *cam = viewpoints().front();
//    glm::vec3 cameraCenterOfFocus(cam->worldTransform() * cam->centerOfFocus());
//    glm::vec3 pixelPos = worldPositionAtDisplayPosition(pixel);
//    Geometry::printVector(pixelPos);
//    std::cout << std::endl;

//    Geometry::Ray ray(cameraCenterOfFocus, glm::normalize(pixelPos - cameraCenterOfFocus));
//    //ray.print();
//    return ray;
    return cam->worldRayAtDisplayPosition(pixel.x, pixel.y);
}

glm::vec3 Display::worldPositionAtDisplayPosition(glm::vec2 pixel)
{
//    Geometry::printVector(glm::vec3(pixel, 0));
//    Geometry::printVector(glm::vec3(resolution(), 0));
//    Geometry::printVector(glm::vec3(size(), 0));


    return glm::vec3(worldTransform() * glm::vec4(((pixel / glm::vec2(resolution())) - glm::vec2(0.5)) * size(), 0 , 1));
}

glm::ivec2 Display::resolution()
{
    return glContext()->defaultFramebufferSize();
}







OpenGLContext *Display::glContext() const
{
    return m_glContext;
}

void Display::setGlContext(OpenGLContext *glContext)
{
    m_glContext = glContext;
}
