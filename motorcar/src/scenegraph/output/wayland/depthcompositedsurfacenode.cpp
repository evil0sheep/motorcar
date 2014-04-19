#include "depthcompositedsurfacenode.h"
#include "../display/display.h"

using namespace motorcar;


//static const GLfloat defaultTextureCoordinates[] = {
//    0, 1,
//    1, 1,
//    1, 0,
//    0, 0,
//};

DepthCompositedSurfaceNode::DepthCompositedSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform)
    :WaylandSurfaceNode(surface, parent, transform)
{
//    static const GLfloat vertexCoordinates[] ={
//        -1, -1, 0,
//        -1, 3, 0,
//        1, 3, 0,
//        1, -1, 0
//    };

    const GLfloat vertexCoordinates[] ={
       -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
       -1.0f,  1.0f, 0.0f
    };



    glGenBuffers(1, &m_surfaceTextureCoordinates);
//    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
//    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), textureCoordinates, GL_STATIC_DRAW);

    glGenBuffers(1, &m_surfaceVertexCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertexCoordinates, GL_STATIC_DRAW);



    glGenFramebuffers(1, &m_frameBuffer);

    glGenTextures(1, &m_colorBufferTexture);
    glBindTexture(GL_TEXTURE_2D, m_colorBufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &m_depthBufferTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthBufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


Geometry::RaySurfaceIntersection *DepthCompositedSurfaceNode::intersectWithSurfaces(const Geometry::Ray &ray)
{
    return SceneGraphNode::intersectWithSurfaces(ray);
}

void DepthCompositedSurfaceNode::prepareFrameBufferForDrawing(Display *display)
{

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

    glBindTexture(GL_TEXTURE_2D, m_colorBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, display->size().x, display->size().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBufferTexture, 0);

    glBindTexture(GL_TEXTURE_2D, m_depthBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, display->size().x, display->size().y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBufferTexture, 0);

    switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)){
            case(GL_FRAMEBUFFER_COMPLETE):
                //std::cout << "Framebuffer Complete" << std::endl;
                break;
            case(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT):
                std::cout << "Framebuffer Attachment Incomplete" << std::endl;
                break;
//            case(GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS):
//                std::cout << "Framebuffer Dimensions Incomplete" << std::endl;
//                break;
            case(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT):
                std::cout << "Framebuffer Attachment Incomplete/Missing" << std::endl;
                break;
            case(GL_FRAMEBUFFER_UNSUPPORTED):
                std::cout << "Framebuffer Unsupported" << std::endl;
                break;
    }

    glEnable(GL_TEXTURE_2D);


}

void DepthCompositedSurfaceNode::drawFrameBufferContents(Display *display)
{
    glBindFramebuffer(GL_FRAMEBUFFER, display->activeFrameBuffer());

    glBindTexture(GL_TEXTURE_2D, m_colorBufferTexture);

    for(ViewPoint *viewpoint : display->viewpoints()){

        viewpoint->viewport()->set();


        glm::vec4 vp = viewpoint->viewport()->viewportParams();

        const GLfloat clientColorTextureCoordinates[] = {
            vp.x, vp.y,
            vp.x + vp.z, vp.y,
            vp.x + vp.z, vp.y + vp.w,
            vp.x, vp.y + vp.w,
        };

//        const GLfloat clientColorTextureCoordinates[] = {
//            vp.x, vp.y + vp.w,
//            vp.x + vp.z, vp.y + vp.w,
//            vp.x + vp.z, vp.y,
//            vp.x, vp.y,
//        };


        glEnableVertexAttribArray(h_aTexCoord_surface);
        glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), clientColorTextureCoordinates, GL_STATIC_DRAW);
        glVertexAttribPointer(h_aTexCoord_surface, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    }

}

void DepthCompositedSurfaceNode::draw(Scene *scene, Display *display)
{


//    for(GLfloat f : textureCoordinates){
//        std::cout << f << ", ";
//    }
//    std::cout <<std::endl;

    GLuint texture = this->surface()->texture();


    glUseProgram(m_surfaceShader->handle());

    glEnableVertexAttribArray(h_aPosition_surface);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glVertexAttribPointer(h_aPosition_surface, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glUniformMatrix4fv(h_uMVPMatrix_surface, 1, GL_FALSE, glm::value_ptr(glm::mat4()));

    this->prepareFrameBufferForDrawing(display);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glBindTexture(GL_TEXTURE_2D, texture);

//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    for(ViewPoint *viewpoint : display->viewpoints()){

        viewpoint->viewport()->set();

        ViewPort *viewport = viewpoint->clientColorViewport();
        glm::vec4 vp = viewport->viewportParams();


        const GLfloat clientColorTextureCoordinates[] = {
            vp.x, 1 - vp.y,
            vp.x + vp.z, 1 - vp.y,
            vp.x + vp.z, 1 - (vp.y + vp.w),
            vp.x, 1 - (vp.y + vp.w),
        };


        glEnableVertexAttribArray(h_aTexCoord_surface);
        glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), clientColorTextureCoordinates, GL_STATIC_DRAW);
        glVertexAttribPointer(h_aTexCoord_surface, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

//        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//        glDrawPixels(viewpoint->clientDepthViewport()->size().x, viewpoint->clientDepthViewport()->size().y, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, texture);
//        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

     this->drawFrameBufferContents(display);


    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(h_aPosition_surface);
    glDisableVertexAttribArray(h_aTexCoord_surface);

    glUseProgram(0);
}













