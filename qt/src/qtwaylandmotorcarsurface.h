#ifndef QTWAYLANDMOTORCARSURFACE_H
#define QTWAYLANDMOTORCARSURFACE_H

#include "../../motorcar/src/motorcar.h"
#include "opengldata.h"
#include "qwaylandsurface.h"
#include "qtwaylandmotorcarcompositor.h"

#include <qwaylandinput.h>
#include <QtDebug>
#include <glm/glm.hpp>

namespace qtmotorcar{
    class QtWaylandMotorcarCompositor;
    class QtWaylandMotorcarSurface : public motorcar::WaylandSurface
    {
    public:
        QtWaylandMotorcarSurface(QWaylandSurface *surface, QtWaylandMotorcarCompositor *compositor, motorcar::WaylandSurface::SurfaceType type);
        ~QtWaylandMotorcarSurface(){}

        //inherited from WaylandSurface
        GLuint texture() override;
        glm::ivec2 size() override;
        void prepare() override;
        void sendMouseEvent(MouseEvent eventType, MouseButton buttonId, glm::vec2 localPostion) override;
        bool valid() override;


        QWaylandSurface *surface() const;
        void setSurface(QWaylandSurface *surface);

    private:
        QWaylandSurface *m_surface;

        GLuint m_textureID;

        QtWaylandMotorcarCompositor *m_compositor;

        GLuint composeSurface(QWaylandSurface *surface, OpenGLData *glData);
        void paintChildren(QWaylandSurface *surface, QWaylandSurface *window, OpenGLData *glData);
        void computeSurfaceTransform(float ppcm);



    };
}


#endif // QTWAYLANDMOTORCARSURFACE_H
