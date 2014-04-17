#include "depthcompositedsurfacenode.h"
#include "../display/display.h"

using namespace motorcar;
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
        1.0f,  3.0f, 0.0f,
       -1.0f,  3.0f, 0.0f
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

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    for(ViewPoint *viewpoint : display->viewpoints()){
        viewpoint->viewport()->set();

        glm::vec4 vp = viewpoint->clientColorViewport()->viewportParams();

        const GLfloat newTextureCoordinates[] = {
            vp.x, vp.y + vp.w,
            vp.x + vp.z, vp.y + vp.w,
            vp.x + vp.z, vp.y,
            vp.x, vp.y,
        };

        glEnableVertexAttribArray(h_aTexCoord_surface);
        glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), newTextureCoordinates, GL_STATIC_DRAW);
        glVertexAttribPointer(h_aTexCoord_surface, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    }


    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(h_aPosition_surface);
    glDisableVertexAttribArray(h_aTexCoord_surface);

    glUseProgram(0);
}











