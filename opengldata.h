#ifndef OPENGLDATA_H
#define OPENGLDATA_H

#include "scenegraphnode.h"
#include "textureblitter.h"
#include "qopenglwindow.h"
/****************************************************************************
**This file is part of the MotorCar QtWayland Compositor
**
**
**Copyright (C) 2013 Forrest Reiling
**
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/

#include "geometry.h"


#include <QtGui/private/qopengltexturecache_p.h>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QLinkedList>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>

#include <glm/glm.hpp>

class SceneGraphNode;
class OpenGLData
{
public:
    QOpenGLWindow *m_window;
    TextureBlitter *m_textureBlitter;
    QOpenGLTextureCache *m_textureCache;
    GLuint m_surface_fbo;
    QOpenGLShaderProgram *m_surfaceShader;
    SceneGraphNode *m_cameraNode;
    Geometry::Camera *m_camera;


    OpenGLData(QOpenGLWindow *window, SceneGraphNode *camera);
    ~OpenGLData();
    QImage makeBackgroundImage(const QString &fileName);

    void calculateVPMatrix();

    float ppcm();

    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;
    glm::mat4 viewProjectionMatrix() const;

private:
    float m_ppcm;
    glm::mat4 m_viewMatrix, m_projectionMatrix, m_viewProjectionMatrix;


};

#endif // OPENGLDATA_H
