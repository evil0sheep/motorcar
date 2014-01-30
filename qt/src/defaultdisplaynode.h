#ifndef DEFAULTDISPLAYNODE_H
#define DEFAULTDISPLAYNODE_H

#include "../../motorcar/src/motorcar.h"
#include "qtwaylandmotorcarsurface.h"
#include <glm/gtc/type_ptr.hpp>




namespace qtmotorcar {
class DefaultDisplayNode : public motorcar::Display
{
public:
    DefaultDisplayNode(motorcar::Scene *scene, OpenGLData *glInfo);




    //inherited from Display
    void prepare() override;
    void renderSurfaceNode(motorcar::WaylandSurfaceNode *surfaceNode, motorcar::GLCamera *camera) override;
    motorcar::Geometry::Ray worldRayAtDisplayPosition(float pixelX, float pixelY) override;
    glm::ivec2 size() override;

    OpenGLData *glInfo() const;
    void setGlInfo(OpenGLData *glInfo);



private:
    OpenGLData *m_glInfo;


};
}


#endif // DEFAULTDISPLAYNODE_H
