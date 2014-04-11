#include "display.h"

using namespace motorcar;

Display::Display(OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
    ,m_glContext(glContext)
    ,m_surfaceShader(new motorcar::OpenGLShader(std::string("../motorcar/src/shaders/motorcarsurface.vert"), std::string("../motorcar/src/shaders/motorcarsurface.frag")))
    ,m_lineShader(new motorcar::OpenGLShader(std::string("../motorcar/src/shaders/motorcarline.vert"), std::string("../motorcar/src/shaders/motorcarline.frag")))
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

    glGenBuffers(1, &m_surfaceTextureCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), textureCoordinates, GL_STATIC_DRAW);

    glGenBuffers(1, &m_surfaceVertexCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertexCoordinates, GL_STATIC_DRAW);

    glGenBuffers(1, &m_lineVertexCoordinates);

    h_aPosition_surface =  glGetAttribLocation(m_surfaceShader->handle(), "aPosition");
    h_aTexCoord_surface =  glGetAttribLocation(m_surfaceShader->handle(), "aTexCoord");
    h_uMVPMatrix_surface  = glGetUniformLocation(m_surfaceShader->handle(), "uMVPMatrix");

    if(h_aPosition_surface < 0 || h_aTexCoord_surface < 0 || h_uMVPMatrix_surface < 0 ){
       std::cout << "problem with surface shader handles: " << h_aPosition_surface << ", "<< h_aTexCoord_surface << ", " << h_uMVPMatrix_surface << std::endl;
    }

    h_aPosition_line =  glGetAttribLocation(m_lineShader->handle(), "aPosition");
    h_uColor_line =  glGetUniformLocation(m_lineShader->handle(), "uColor");
    h_uMVPMatrix_line  = glGetUniformLocation(m_lineShader->handle(), "uMVPMatrix");

    if(h_aPosition_line < 0 || h_uColor_line < 0 || h_uMVPMatrix_line < 0 ){
       std::cout << "problem with line shader handles: " << h_aPosition_line << ", "<< h_uColor_line << ", " << h_uMVPMatrix_line << std::endl;
    }


}

Display::~Display()

{
    delete m_surfaceShader;
}

void Display::prepareForDraw()
{
   //std::cout << "display size: " << this->resolution().x << ", " << this->resolution().y <<std::endl;
//    Geometry::printMatrix(worldTransform());

//    for(ViewPoint *viewpoint : viewpoints()){
//        viewpoint->updateViewMatrix();
//    }
    glContext()->makeCurrent();
    glClearColor(.7f, .85f, 1.f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc (GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
}

void Display::renderDrawable(Drawable *drawable)
{
    for(ViewPoint *viewpoint : m_viewpoints){
        drawable->drawViewpoint(viewpoint);
    }
}

void Display::addViewpoint(ViewPoint *v)
{
    m_viewpoints.push_back(v);
}


std::vector<ViewPoint *> Display::viewpoints() const
{
    return m_viewpoints;
}

glm::vec2 Display::size() const
{
    return m_size;
}


void Display::renderSurfaceNode(WaylandSurfaceNode *surfaceNode, ViewPoint *viewpoint)
{
    viewpoint->viewport()->set();

    GLuint texture = surfaceNode->surface()->texture();

    glUseProgram(m_surfaceShader->handle());

    glEnableVertexAttribArray(h_aPosition_surface);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glVertexAttribPointer(h_aPosition_surface, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(h_aTexCoord_surface);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
    glVertexAttribPointer(h_aTexCoord_surface, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glUniformMatrix4fv(h_uMVPMatrix_surface, 1, GL_FALSE, glm::value_ptr(viewpoint->projectionMatrix() * viewpoint->viewMatrix() *  surfaceNode->worldTransform() * surfaceNode->surfaceTransform()));

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(h_aPosition_surface);
    glDisableVertexAttribArray(h_aTexCoord_surface);

    glUseProgram(0);

}

void Display::renderDepthCompositedSurfaceNode(DepthCompositedSurfaceNode *surfaceNode, ViewPoint *viewpoint)
{
    viewpoint->viewport()->set();

    glm::vec4 vp = viewpoint->viewport()->viewportParams();

    const GLfloat textureCoordinates[] = {
        vp.x, vp.y + vp.w,
        vp.x, vp.y,
        vp.x + vp.z, vp.y,
        vp.x + vp.z, vp.y + vp.w,
    };

//    for(GLfloat f : textureCoordinates){
//        std::cout << f << ", ";
//    }
//    std::cout <<std::endl;

    GLuint texture = surfaceNode->surface()->texture();

    glUseProgram(m_surfaceShader->handle());

    glEnableVertexAttribArray(h_aPosition_surface);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glVertexAttribPointer(h_aPosition_surface, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(h_aTexCoord_surface);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), textureCoordinates, GL_STATIC_DRAW);
    glVertexAttribPointer(h_aTexCoord_surface, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glUniformMatrix4fv(h_uMVPMatrix_surface, 1, GL_FALSE, glm::value_ptr(surfaceNode->surfaceTransform()));

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(h_aPosition_surface);
    glDisableVertexAttribArray(h_aTexCoord_surface);

    glUseProgram(0);
}

void Display::renderWireframeNode(WireframeNode *node, ViewPoint *camera)
{
    camera->viewport()->set();

    glUseProgram(m_lineShader->handle());

    glEnableVertexAttribArray(h_aPosition_line);
    glBindBuffer(GL_ARRAY_BUFFER, m_lineVertexCoordinates);
    glVertexAttribPointer(h_aPosition_line, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBufferData(GL_ARRAY_BUFFER,  node->numSegments() * 2 * 3 * sizeof(float), node->segments(), GL_DYNAMIC_DRAW);

    glUniform3fv(h_uColor_line, 1, glm::value_ptr(node->lineColor()));
    glUniformMatrix4fv(h_uMVPMatrix_line, 1, GL_FALSE, glm::value_ptr(camera->projectionMatrix() * camera->viewMatrix() *  node->worldTransform()));


    glDrawArrays(GL_LINES, 0, 2 * node->numSegments());

    glDisableVertexAttribArray(h_aPosition_line);

    glUseProgram(0);

}



Geometry::Ray Display::worldRayAtDisplayPosition(glm::vec2 pixel)
{
    ViewPoint *cam = viewpoints().front();
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
