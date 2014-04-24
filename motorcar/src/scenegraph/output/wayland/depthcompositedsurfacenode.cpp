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





}


Geometry::RaySurfaceIntersection *DepthCompositedSurfaceNode::intersectWithSurfaces(const Geometry::Ray &ray)
{
    return SceneGraphNode::intersectWithSurfaces(ray);
}



void DepthCompositedSurfaceNode::drawFrameBufferContents(Display *display)
{


    //glBindTexture(GL_TEXTURE_2D, m_colorBufferTexture);


    glBindFramebuffer(GL_READ_FRAMEBUFFER, display->scratchFrameBuffer());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, display->activeFrameBuffer());
    glm::ivec2 res = display->size();
    glBlitFramebuffer(0, 0, res.x - 1, res.y - 1, 0, 0, res.x - 1 , res.y - 1, GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT, GL_NEAREST);

//    //printOpenGLError ();
//    int error;
//    do{
//         error = glGetError();
//        switch(error){
//            case(GL_INVALID_OPERATION):
//                std::cout << "GL_INVALID_OPERATION" <<std::endl;
//                break;
//            case(GL_INVALID_FRAMEBUFFER_OPERATION):
//                std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" <<std::endl;
//                break;
//            case(GL_INVALID_VALUE):
//                std::cout << "GL_INVALID_VALUE" <<std::endl;
//                break;
//            case(GL_NO_ERROR):
//                std::cout << "GL_NO_ERROR" <<std::endl;
//                break;
//            default:
//                std::cout << "default" <<std::endl;

//        }
//    }while (error != GL_NO_ERROR);


    //std::cout << glGetError() <<std::endl <<std::endl;



}

void DepthCompositedSurfaceNode::draw(Scene *scene, Display *display)
{




    GLuint texture = this->surface()->texture();


    glUseProgram(m_surfaceShader->handle());

    glEnableVertexAttribArray(h_aPosition_surface);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glVertexAttribPointer(h_aPosition_surface, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glUniformMatrix4fv(h_uMVPMatrix_surface, 1, GL_FALSE, glm::value_ptr(glm::mat4()));


    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, display->scratchFrameBuffer());

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glBindTexture(GL_TEXTURE_2D, texture);



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



    glBindFramebuffer(GL_FRAMEBUFFER, display->activeFrameBuffer());

    glDisableVertexAttribArray(h_aPosition_surface);
    glDisableVertexAttribArray(h_aTexCoord_surface);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);


}













