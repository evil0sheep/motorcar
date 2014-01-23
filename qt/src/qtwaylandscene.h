#ifndef QTWAYLANDSCENE_H
#define QTWAYLANDSCENE_H
#include "../../motorcar/src/motorcar.h"
#include "qtwaylandsurfacenode.h"

using namespace motorcar;

namespace qtmotorcar {
class QtWaylandScene : public Scene
{
public:
    QtWaylandScene();

    QtwaylandSurfaceNode *getSurfaceNode(const QWaylandSurface *surface);
};
}


#endif // QTWAYLANDSCENE_H
