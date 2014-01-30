#ifndef QTWAYLANDSCENE_H
#define QTWAYLANDSCENE_H
#include "../../motorcar/src/motorcar.h"
#include "qwaylandsurface.h"
#include "qtwaylandmotorcarsurface.h"
#include <map>



namespace qtmotorcar {
class QtWaylandMotorcarScene : public motorcar::Scene
{
public:
    QtWaylandMotorcarScene();
    ~QtWaylandMotorcarScene(){}

    //inhereted from scene
    //calls superclass equivilent and then adds WaylandNodes to an associative map
    //for later retrieval by surface
    void notifyNodeAdded(motorcar::SceneGraphNode *node) override;

    motorcar::WaylandSurfaceNode *getSurfaceNode(QWaylandSurface *surface = NULL);

 private:
    std::map<QWaylandSurface *, motorcar::WaylandSurfaceNode *> m_surfaceMap;
};
}


#endif // QTWAYLANDSCENE_H
