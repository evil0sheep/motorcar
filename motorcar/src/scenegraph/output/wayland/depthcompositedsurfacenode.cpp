#include "depthcompositedsurfacenode.h"
#include "../display/display.h"

using namespace motorcar;

DepthCompositedSurfaceNode::DepthCompositedSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform, glm::vec3 dimensions)
    :m_resource (NULL)
    ,WaylandSurfaceNode(surface, parent, transform)
    ,m_depthCompositedSurfaceShader(new motorcar::OpenGLShader(std::string("../motorcar/src/shaders/depthcompositedsurface.vert"), std::string("../motorcar/src/shaders/depthcompositedsurface.frag")))
    ,m_depthCompositedSurfaceBlitter(new motorcar::OpenGLShader(std::string("../motorcar/src/shaders/depthcompositedsurfaceblitter.vert"), std::string("../motorcar/src/shaders/depthcompositedsurfaceblitter.frag")))
    ,m_dimensions(dimensions)
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



    glGenBuffers(1, &m_colorTextureCoordinates);
    glGenBuffers(1, &m_depthTextureCoordinates);
//    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
//    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), textureCoordinates, GL_STATIC_DRAW);

    glGenBuffers(1, &m_surfaceVertexCoordinates);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertexCoordinates, GL_STATIC_DRAW);



    h_aPosition =  glGetAttribLocation(m_depthCompositedSurfaceShader->handle(), "aPosition");
    h_aColorTexCoord =  glGetAttribLocation(m_depthCompositedSurfaceShader->handle(), "aColorTexCoord");
    h_aDepthTexCoord =  glGetAttribLocation(m_depthCompositedSurfaceShader->handle(), "aDepthTexCoord");

    if(h_aPosition < 0 || h_aColorTexCoord < 0 || h_aDepthTexCoord < 0 ){
       std::cout << "problem with depth compositing shader handles: " << h_aPosition << ", "<< h_aColorTexCoord << ", " << h_aDepthTexCoord << std::endl;
    }


    h_aPosition_blit =  glGetAttribLocation(m_depthCompositedSurfaceBlitter->handle(), "aPosition");
    h_aTexCoord_blit =  glGetAttribLocation(m_depthCompositedSurfaceBlitter->handle(), "aTexCoord");
    h_uColorSampler_blit = glGetUniformLocation(m_depthCompositedSurfaceBlitter->handle(), "uColorSampler");
    h_uDepthSampler_blit = glGetUniformLocation(m_depthCompositedSurfaceBlitter->handle(), "uDepthSampler");

    if(h_aPosition_blit < 0 || h_aTexCoord_blit < 0 || h_uColorSampler_blit < 0 || h_uDepthSampler_blit < 0 ){
       std::cout << "problem with depth blitting shader handles: " <<
                    h_aPosition_blit << ", "<< h_aTexCoord_blit << ", " << h_uColorSampler_blit << h_uDepthSampler_blit << std::endl;
    }


    glUseProgram(m_depthCompositedSurfaceBlitter->handle());

    glUniform1i(h_uColorSampler_blit, 0); //Texture unit 0 is for color maps.
    glUniform1i(h_uDepthSampler_blit, 1); //Texture unit 1 is for depth maps.

    glUseProgram(0);



    wl_array_init(&m_dimensionsArray);
    wl_array_init(&m_transformArray);

    wl_array_add(&m_dimensionsArray, sizeof(glm::vec3));
    wl_array_add(&m_transformArray, sizeof(glm::mat4));



}


Geometry::RaySurfaceIntersection *DepthCompositedSurfaceNode::intersectWithSurfaces(const Geometry::Ray &ray)
{
    return SceneGraphNode::intersectWithSurfaces(ray);
}



void DepthCompositedSurfaceNode::drawFrameBufferContents(Display *display)
{



    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    //glBindTexture(GL_TEXTURE_2D, m_colorBufferTexture);


//    glBindFramebuffer(GL_READ_FRAMEBUFFER, display->scratchFrameBuffer());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, display->activeFrameBuffer());


//    glm::ivec2 res = display->size();
    //glBlitFramebuffer(0, 0, res.x - 1, res.y - 1, 0, 0, res.x - 1 , res.y - 1, GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    //only for default display
    //glGetError();

    glUseProgram(m_depthCompositedSurfaceBlitter->handle());

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, display->scratchColorBufferTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, display->scratchDepthBufferTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glEnableVertexAttribArray(h_aPosition_blit);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glVertexAttribPointer(h_aPosition_blit, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(h_aTexCoord_blit);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    for(ViewPoint *viewpoint : display->viewpoints()){

        viewpoint->viewport()->set();

        glm::vec4 vp = viewpoint->viewport()->viewportParams();

        const GLfloat textureBlitCoordinates[] = {
            vp.x, vp.y,
            vp.x + vp.z,  vp.y,
            vp.x + vp.z,  (vp.y + vp.w),
            vp.x, (vp.y + vp.w),
        };


        glVertexAttribPointer(h_aTexCoord_blit, 2, GL_FLOAT, GL_FALSE, 0, textureBlitCoordinates);


        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    }

}





void DepthCompositedSurfaceNode::draw(Scene *scene, Display *display)
{




    GLuint texture = this->surface()->texture();


    glUseProgram(m_depthCompositedSurfaceShader->handle());

    glEnableVertexAttribArray(h_aPosition);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(h_aColorTexCoord);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(h_aDepthTexCoord);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, display->scratchFrameBuffer());

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    glm::vec4 vp;
    for(ViewPoint *viewpoint : display->viewpoints()){

        viewpoint->viewport()->set();

        vp = viewpoint->clientColorViewport()->viewportParams();

        const GLfloat clientColorTextureCoordinates[] = {
            vp.x, 1 - vp.y,
            vp.x + vp.z, 1 - vp.y,
            vp.x + vp.z, 1 - (vp.y + vp.w),
            vp.x, 1 - (vp.y + vp.w),
        };



//        glEnableVertexAttribArray(h_aColorTexCoord);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindBuffer(GL_ARRAY_BUFFER, m_colorTextureCoordinates);
//        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), clientColorTextureCoordinates, GL_STATIC_DRAW);
//        glVertexAttribPointer(h_aColorTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(h_aColorTexCoord, 2, GL_FLOAT, GL_FALSE, 0, clientColorTextureCoordinates);

        vp = viewpoint->clientDepthViewport()->viewportParams();

        const GLfloat clientDepthTextureCoordinates[] = {
            vp.x, 1 - vp.y,
            vp.x + vp.z, 1 - vp.y,
            vp.x + vp.z, 1 - (vp.y + vp.w),
            vp.x, 1 - (vp.y + vp.w),
        };


//        glEnableVertexAttribArray(h_aDepthTexCoord);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindBuffer(GL_ARRAY_BUFFER, m_depthTextureCoordinates);
//        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), clientDepthTextureCoordinates, GL_STATIC_DRAW);
//        glVertexAttribPointer(h_aDepthTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(h_aDepthTexCoord, 2, GL_FLOAT, GL_FALSE, 0, clientDepthTextureCoordinates);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

//        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//        glDrawPixels(viewpoint->clientDepthViewport()->size().x, viewpoint->clientDepthViewport()->size().y, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, texture);
//        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }


    this->drawFrameBufferContents(display);



    glBindFramebuffer(GL_FRAMEBUFFER, display->activeFrameBuffer());

    glDisableVertexAttribArray(h_aPosition);
    glDisableVertexAttribArray(h_aColorTexCoord);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);




}





void DepthCompositedSurfaceNode::handle_set_size_3d(struct wl_client *client,
                struct wl_resource *resource,
                struct wl_array *dimensions){
    DepthCompositedSurfaceNode *surfaceNode = static_cast<DepthCompositedSurfaceNode *> (resource->data);
    if(dimensions->size != 3 * sizeof(float)){
        //error conditions
        std::cerr << "Error: Dimensions array has wrong size: " << dimensions->size << std::endl;

    }else{
        glm::vec3 dims = glm::make_vec3((float *)(dimensions->data));
        std::cout << "Client resized 3D window to: ";
        Geometry::printVector(dims);
        surfaceNode->setDimensions(dims);
    }
}



const static struct motorcar_surface_interface motorcarSurfaceInterface = {
    DepthCompositedSurfaceNode::handle_set_size_3d
};

glm::vec3 DepthCompositedSurfaceNode::dimensions() const
{
    return m_dimensions;
}

void DepthCompositedSurfaceNode::sendTransformToClient()
{

    if(m_resource != NULL){
        glm::mat4 transform = this->worldTransform();
        std::cout << "sending new tranform to client :" <<std::endl;
        Geometry::printMatrix(transform);
        std::memcpy(m_transformArray.data, glm::value_ptr(transform), m_transformArray.size);
        motorcar_surface_send_transform_matrix(m_resource, &m_transformArray);
    }


}

void DepthCompositedSurfaceNode::handleWorldTransformChange(Scene *scene)
{
    sendTransformToClient();
}


void DepthCompositedSurfaceNode::requestSize3D(const glm::vec3 &dimensions)
{
    if(m_resource != NULL){
        std::cout << "Requesting client resize 3D window to: ";
        Geometry::printVector(dimensions);

        std::memcpy(m_dimensionsArray.data, glm::value_ptr(m_dimensions), m_dimensionsArray.size);

        motorcar_surface_send_request_size_3d(m_resource, &m_dimensionsArray);
    }

}




void DepthCompositedSurfaceNode::setDimensions(const glm::vec3 &dimensions)
{
    m_dimensions = dimensions;
}




wl_resource *DepthCompositedSurfaceNode::resource() const
{
    return m_resource;
}

void DepthCompositedSurfaceNode::configureResource(wl_client *client, uint32_t id)
{
    m_resource = wl_resource_create(client, &motorcar_surface_interface, motorcar_surface_interface.version, id);
    wl_resource_set_implementation(m_resource, &motorcarSurfaceInterface, this, 0);
    sendTransformToClient();
    requestSize3D(m_dimensions);
}














