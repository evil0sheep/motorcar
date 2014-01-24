#include "qtwaylandmotorcarscene.h"

using namespace qtmotorcar;

QtWaylandMotorcarScene::QtWaylandMotorcarScene()
    :m_surfaceMap()
{
}


void QtWaylandMotorcarScene::notifyNodeAdded(motorcar::SceneGraphNode *node)
{
    motorcar::Scene::notifyNodeAdded(node);
    motorcar::WaylandSurfaceNode *surfaceNode = dynamic_cast<motorcar::WaylandSurfaceNode *>(node);
    if(surfaceNode != NULL){
        m_surfaceMap.insert(std::pair<motorcar::WaylandSurface *, motorcar::WaylandSurfaceNode *>(surfaceNode->surface(), surfaceNode));
    }

}

motorcar::WaylandSurfaceNode *QtWaylandMotorcarScene::getSurfaceNode(const motorcar::WaylandSurface *surface)
{
    //if passed NULL return first Surface
    if(surface == NULL){
        if(!m_surfaceMap.empty()){
            return m_surfaceMap.begin()->second;
        }else{
            return NULL;
        }
    }
    if(m_surfaceMap.count(surface)){
        return m_surfaceMap.find(surface)->second;
    }
    return NULL;
}
