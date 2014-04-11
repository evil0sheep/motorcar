#include "rendertotexturedisplay.h"

using namespace motorcar;

RenderToTextureDisplay::RenderToTextureDisplay(float scale, glm::vec4 distortionK, OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform)
    :Display(glContext, displayDimensions, parent, transform)
    ,m_scale(scale)
    ,m_distortionK(distortionK)
    ,m_distortionShader(new motorcar::OpenGLShader("../motorcar/src/shaders/motorcarbarreldistortion.vert", "../motorcar/src/shaders/motorcarbarreldistortion.frag"))
{

    h_aPosition_distortion =  glGetAttribLocation(m_distortionShader->handle(), "aPosition");
    h_aTexCoord_distortion =  safe_glGetAttribLocation(m_distortionShader->handle(), "aTexCoord");
    h_uDistortionK =  glGetUniformLocation(m_distortionShader->handle(), "uDistortionK");
    h_uLenseCenter =  glGetUniformLocation(m_distortionShader->handle(), "uLenseCenter");
    h_uViewportParams =  glGetUniformLocation(m_distortionShader->handle(), "uViewportParams");
    h_uScaleFactor = glGetUniformLocation(m_distortionShader->handle(), "uScaleFactor");

    //printOpenGLError();

    if(h_aPosition_distortion < 0 || h_aTexCoord_distortion < 0 || h_uDistortionK < 0 || h_uLenseCenter < 0|| h_uViewportParams < 0 || h_uScaleFactor < 0){
       std::cout << "problem with distortion shader handles: "
                 << h_aPosition_distortion
                 << ", "<< h_aTexCoord_distortion
                 << ", "<< h_uDistortionK
                 << ", "<< h_uLenseCenter
                 << ", "<< h_uViewportParams
                 << ", "<< h_uScaleFactor
                 << std::endl;
    }


    const GLfloat vertexCoordinates[] ={
       -1.0f, -1.0f, 0.0f,
       -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    glGenBuffers(1, &m_surfaceTextureCoordinates);

    glGenBuffers(1, &m_surfaceVertexCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertexCoordinates, GL_STATIC_DRAW);


    glm::ivec2 res = resolution();

    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

    glGenTextures(1, &m_frameBufferTexture);
    glBindTexture(GL_TEXTURE_2D, m_frameBufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Allocate space for the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res.x, res.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frameBufferTexture, 0);


    glGenRenderbuffers(1, &m_depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, res.x, res.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
    glEnable(GL_TEXTURE_2D);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

RenderToTextureDisplay::~RenderToTextureDisplay()
{
    glDeleteFramebuffers(1, &m_frameBuffer);
    delete m_distortionShader;


}

void RenderToTextureDisplay::prepareForDraw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    Display::prepareForDraw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderToTextureDisplay::finishDraw()
{

    glUseProgram(m_distortionShader->handle());

    //std::cout << "distortion shader handle " << m_distortionShader->handle() << std::endl;

    glEnableVertexAttribArray(h_aPosition_distortion);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glVertexAttribPointer(h_aPosition_distortion, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindTexture(GL_TEXTURE_2D, m_frameBufferTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLfloat texCoords [8];

    float temp_scale = m_scale;
    m_scale = 1;

    for(ViewPoint *cam : viewpoints()){

        cam->viewport()->uvCoords(texCoords);
        cam->viewport()->set();


        glUniform2fv(h_uLenseCenter, 1, glm::value_ptr(glm::vec2(cam->centerOfFocus())));
        glUniform4fv(h_uViewportParams, 1, glm::value_ptr(cam->viewport()->viewportParams()));
        glUniform4fv(h_uDistortionK, 1, glm::value_ptr(m_distortionK));
        glUniform1f(h_uScaleFactor, temp_scale);


        glEnableVertexAttribArray(h_aTexCoord_distortion);
        glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), texCoords, GL_STATIC_DRAW);
        glVertexAttribPointer(h_aTexCoord_distortion, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    }
    m_scale = temp_scale;

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(h_aPosition_distortion);
    glDisableVertexAttribArray(h_aTexCoord_distortion);

    glUseProgram(0);
}

void RenderToTextureDisplay::renderSurfaceNode(WaylandSurfaceNode *surfaceNode, ViewPoint *camera)
{

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    Display::renderSurfaceNode(surfaceNode, camera);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderToTextureDisplay::renderDepthCompositedSurfaceNode(DepthCompositedSurfaceNode *surfaceNode, ViewPoint *camera)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    Display::renderDepthCompositedSurfaceNode(surfaceNode, camera);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderToTextureDisplay::renderWireframeNode(WireframeNode *node, ViewPoint *camera)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    Display::renderWireframeNode(node, camera);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::ivec2 RenderToTextureDisplay::resolution()
{
    return glm::ivec2(m_scale * glm::vec2(Display::resolution()));
}

glm::vec2 RenderToTextureDisplay::size() const
{
    return m_scale * Display::size();
}
