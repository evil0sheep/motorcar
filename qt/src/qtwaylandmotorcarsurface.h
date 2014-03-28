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
        ///Get the position of this surface in parent surface-local coordinates
        glm::ivec2 position() override;
        ///return the parent surface
        WaylandSurface *parentSurface() override;

        void prepare() override;      
        void sendEvent(const motorcar::Event &event) override;
        bool valid() override;


        QWaylandSurface *surface() const;
        void setSurface(QWaylandSurface *surface);

    private:
        QWaylandSurface *m_surface;
        bool m_ownsTexture;
        GLuint m_textureID;

        QtWaylandMotorcarCompositor *m_compositor;

        GLuint composeSurface(QWaylandSurface *surface, bool *textureOwned, OpenGLData *glData);
        void paintChildren(QWaylandSurface *surface, QWaylandSurface *window, const QSize &windowSize, OpenGLData *glData);
        void computeSurfaceTransform(float ppcm);



    };
}


#endif // QTWAYLANDMOTORCARSURFACE_H
