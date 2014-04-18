#include "waylandsurfacenode.h"
#include "../display/display.h"
#include "../../../gl/viewport.h"
using namespace motorcar;

WaylandSurfaceNode::WaylandSurfaceNode(WaylandSurface *surface, SceneGraphNode *parent, const glm::mat4 &transform)
    :Drawable(parent, transform)
    ,m_surfaceShader(new motorcar::OpenGLShader(std::string("../motorcar/src/shaders/motorcarsurface.vert"), std::string("../motorcar/src/shaders/motorcarsurface.frag")))

{

     std::cout << std::endl << "constructing surface node " << this << std::endl;
    this->setSurface(surface);
    static const GLfloat textureCoordinates[] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0
    };
    static const GLfloat vertexCoordinates[] ={
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



    h_aPosition_surface =  glGetAttribLocation(m_surfaceShader->handle(), "aPosition");
    h_aTexCoord_surface =  glGetAttribLocation(m_surfaceShader->handle(), "aTexCoord");
    h_uMVPMatrix_surface  = glGetUniformLocation(m_surfaceShader->handle(), "uMVPMatrix");

    if(h_aPosition_surface < 0 || h_aTexCoord_surface < 0 || h_uMVPMatrix_surface < 0 ){
       std::cout << "problem with surface shader handles: " << h_aPosition_surface << ", "<< h_aTexCoord_surface << ", " << h_uMVPMatrix_surface << std::endl;
    }

}

WaylandSurfaceNode::~WaylandSurfaceNode()
{
    std::cout << "deleting surfaceNode: " << this <<std::endl;
}

WaylandSurface *WaylandSurfaceNode::surface() const
{
    return m_surface;
}

void WaylandSurfaceNode::setSurface(WaylandSurface *surface)
{
    m_surface = surface;
}

glm::mat4 WaylandSurfaceNode::surfaceTransform() const
{
    return m_surfaceTransform;
}

void WaylandSurfaceNode::computeSurfaceTransform(float ppcm)
{
    if(ppcm > 0){
        float ppm = ppcm * 100.f;
        glm::mat4 surfaceRotation = glm::rotate(glm::mat4(1), 180.f ,glm::vec3(0, 0, 1));
        glm::mat4 surfaceScale = glm::scale(glm::mat4(1), glm::vec3( -m_surface->size().x / ppm,  m_surface->size().y / ppm, 1));
        glm::mat4 surfaceOffset = glm::translate(glm::mat4(1), glm::vec3(-0.5f, -0.5f, 0));
        m_surfaceTransform = surfaceRotation * surfaceScale * surfaceOffset  ;
    }
}

bool WaylandSurfaceNode::computeLocalSurfaceIntersection(const Geometry::Ray &localRay, glm::vec2 &localIntersection, float &t)
{
    Geometry::Plane surfacePlane = Geometry::Plane(glm::vec3(0), glm::vec3(0,0,1));
    if(glm::dot(localRay.d, surfacePlane.n) == 0) return false;

    Geometry::Ray transformedRay = localRay.transform(glm::inverse(surfaceTransform()));

    //transformedRay.print();

    t = surfacePlane.intersect(transformedRay);
    //std::cout << "t = " << t << std::endl;
    glm::vec3 intersection = transformedRay.solve(t);

    //Geometry::printVector(glm::vec3(intersection));

    //transformedRay.print();
    //transformedRay.draw(this, glm::vec3(0,0,1),  surfaceTransform());

    glm::vec3 coords= intersection * glm::vec3(m_surface->size().x, m_surface->size().y, 0);
    localIntersection =  glm::vec2(coords);

    return t >= 0;
}

Geometry::RaySurfaceIntersection *WaylandSurfaceNode::intersectWithSurfaces(const Geometry::Ray &ray)
{
    Geometry::RaySurfaceIntersection *closestSubtreeIntersection = SceneGraphNode::intersectWithSurfaces(ray);

    if(((int) m_surface->type()) == WaylandSurface::SurfaceType::CURSOR){
        return closestSubtreeIntersection;
    }

    Geometry::Ray localRay = ray.transform(inverseTransform());

    float t;
    glm::vec2 localIntersection;
    bool isIntersected = computeLocalSurfaceIntersection(localRay, localIntersection, t);


    if(isIntersected && (closestSubtreeIntersection == NULL || t < closestSubtreeIntersection-> t)
            && localIntersection.x >= 0 && localIntersection.x <= m_surface->size().x
            && localIntersection.y >= 0 && localIntersection.y <= m_surface->size().y){

            return new Geometry::RaySurfaceIntersection(this, localIntersection, ray, t);


    }else{
        return closestSubtreeIntersection;
    }
}

void WaylandSurfaceNode::draw(Scene *scene, Display *display)
{
    //std::cout << "drawing surface node " << this <<std::endl;

    GLuint texture = this->surface()->texture();

    glUseProgram(m_surfaceShader->handle());

    glEnableVertexAttribArray(h_aPosition_surface);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexCoordinates);
    glVertexAttribPointer(h_aPosition_surface, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(h_aTexCoord_surface);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceTextureCoordinates);
    glVertexAttribPointer(h_aTexCoord_surface, 2, GL_FLOAT, GL_FALSE, 0, 0);



    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    for(ViewPoint *viewpoint : display->viewpoints()){
        viewpoint->viewport()->set();
        glUniformMatrix4fv(h_uMVPMatrix_surface, 1, GL_FALSE, glm::value_ptr(viewpoint->projectionMatrix() * viewpoint->viewMatrix() *  this->worldTransform() * this->surfaceTransform()));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    }
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(h_aPosition_surface);
    glDisableVertexAttribArray(h_aTexCoord_surface);

    glUseProgram(0);
}

void WaylandSurfaceNode::handleFrameBegin(Scene *scene)
{
    Drawable::handleFrameBegin(scene);
    if(visible()){
        computeSurfaceTransform(8);
        surface()->prepare();

    }

}


bool WaylandSurfaceNode::mapped() const
{
    return m_mapped;
}

void WaylandSurfaceNode::setMapped(bool mapped)
{
    m_mapped = mapped;
}
bool WaylandSurfaceNode::damaged() const
{
    return m_damaged;
}

void WaylandSurfaceNode::setDamaged(bool damaged)
{
    m_damaged = damaged;
}


