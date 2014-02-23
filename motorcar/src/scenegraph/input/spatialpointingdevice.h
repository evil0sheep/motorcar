#ifndef SPATIALMOUSE_H
#define SPATIALMOUSE_H

#include <glm/gtc/type_ptr.hpp>

#include "inputelement.h"
#include "../physicalnode.h"
#include "../output/wireframenode.h"

namespace motorcar {
class SpatialPointingDevice : public InputElement, public PhysicalNode
{
public:
    SpatialPointingDevice(PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~SpatialPointingDevice() {}

    //inhereted from SceneGraphNode
    void traverseNode(Scene *scene, long deltaMillis) override;

protected:
    Geometry::RaySurfaceIntersection *m_latestIntersection;
};
}


#endif // SPATIALMOUSE_H
