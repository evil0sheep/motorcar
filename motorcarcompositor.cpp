/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Compositor.
**
** $QT_BEGIN_LICENSE:BSD$
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
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include "motorcarcompositor.h"



#include <QMouseEvent>
#include <QKeyEvent>
#include <QTouchEvent>
#include <QGuiApplication>
#include <QCursor>
#include <QPixmap>
#include <QScreen>


#include <QtCompositor/qwaylandinput.h>

MotorcarCompositor::MotorcarCompositor(QOpenGLWindow *window)
    : QWaylandCompositor(window, 0, DefaultExtensions | SubSurfaceExtension)
    , m_sceneGraphRoot(new SceneGraphNode(NULL))
    , m_glData(new OpenGLData(window, new SceneGraphNode(m_sceneGraphRoot, glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0,0,1.5f)), 180.f, glm::vec3(0,1,0)))))
    , m_renderScheduler(this)
    , m_draggingWindow(0)
    , m_dragKeyIsPressed(false)
    , m_cursorSurface(0)
    , m_cursorHotspotX(0)
    , m_cursorHotspotY(0)
    , m_modifiers(Qt::NoModifier)

{

    m_renderScheduler.setSingleShot(true);
    connect(&m_renderScheduler,SIGNAL(timeout()),this,SLOT(render()));



    window->installEventFilter(this);

    setRetainedSelectionEnabled(true);

    setOutputGeometry(QRect(QPoint(0, 0), window->size()));
    setOutputRefreshRate(qRound(qGuiApp->primaryScreen()->refreshRate() * 1000.0));
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    //glClearDepth(0.1f);
}

MotorcarCompositor::~MotorcarCompositor()
{
    delete m_glData;
}




//TODO: consider revising to take  MotorcarSurfaceNode as argument depending on call sites
void MotorcarCompositor::ensureKeyboardFocusSurface(QWaylandSurface *oldSurface)
{
    QWaylandSurface *kbdFocus = defaultInputDevice()->keyboardFocus();
    if (kbdFocus == oldSurface || !kbdFocus){
       MotorcarSurfaceNode *n = m_sceneGraphRoot->getSurfaceNode();
       // defaultInputDevice()->setKeyboardFocus(m_surfaces.isEmpty() ? 0 : m_surfaces.last());
       defaultInputDevice()->setKeyboardFocus(n ?  n->surface() : NULL);
    }
}

void MotorcarCompositor::surfaceDestroyed(QObject *object)
{

    QWaylandSurface *surface = static_cast<QWaylandSurface *>(object);
    //m_surfaces.removeOne(surface);
    if(surface != NULL){ //because calling getSurfaceNode with NULL will cause the first surface node to be returned
        delete m_sceneGraphRoot->getSurfaceNode(surface); //will return surfaceNode whose destructor will remove it from the scenegraph
    }
    ensureKeyboardFocusSurface(surface);
    m_renderScheduler.start(0);
}

void MotorcarCompositor::surfaceMapped()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    QPoint pos;
    //if (!m_surfaces.contains(surface)) {
    if (!m_sceneGraphRoot->getSurfaceNode(surface)) {
//        uint px = 0;
//        uint py = 0;
//        if (!QCoreApplication::arguments().contains(QLatin1String("-stickytopleft"))) {
//            px = 1 + (qrand() % (m_window->width() - surface->size().width() - 2));
//            py = 1 + (qrand() % (m_window->height() - surface->size().height() - 2));
//        }
//        pos = QPoint(px, py);
//        surface->setPos(pos);
        surface->setPos(QPoint(0, 0));
        //Sometimes surfaces dont have shell_surfaces, so don't render them
        if (surface->hasShellSurface()) {
            //m_surfaces.append(surface);
            new MotorcarSurfaceNode(m_sceneGraphRoot, surface);
            defaultInputDevice()->setKeyboardFocus(surface);
        }
    }

    m_renderScheduler.start(0);
}

void MotorcarCompositor::surfaceUnmapped()
{

    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
//    if (m_surfaces.removeOne(surface))
//        m_surfaces.insert(0, surface);

    ensureKeyboardFocusSurface(surface);

    //m_renderScheduler.start(0);
}

void MotorcarCompositor::surfaceDamaged(const QRect &rect)
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    surfaceDamaged(surface, rect);
}

void MotorcarCompositor::surfacePosChanged()
{
    m_renderScheduler.start(0);
}

void MotorcarCompositor::surfaceDamaged(QWaylandSurface *surface, const QRect &rect)
{
    Q_UNUSED(surface)
    Q_UNUSED(rect)
    m_renderScheduler.start(0);
}

void MotorcarCompositor::surfaceCreated(QWaylandSurface *surface)
{
    connect(surface, SIGNAL(destroyed(QObject *)), this, SLOT(surfaceDestroyed(QObject *)));
    connect(surface, SIGNAL(mapped()), this, SLOT(surfaceMapped()));
    connect(surface, SIGNAL(unmapped()), this, SLOT(surfaceUnmapped()));
    connect(surface, SIGNAL(damaged(const QRect &)), this, SLOT(surfaceDamaged(const QRect &)));
    connect(surface, SIGNAL(extendedSurfaceReady()), this, SLOT(sendExpose()));
    connect(surface, SIGNAL(posChanged()), this, SLOT(surfacePosChanged()));
    m_renderScheduler.start(0);
}

void MotorcarCompositor::sendExpose()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    surface->sendOnScreenVisibilityChange(true);
}

void MotorcarCompositor::updateCursor()
{
    if (!m_cursorSurface)
        return;
    QCursor cursor(QPixmap::fromImage(m_cursorSurface->image()), m_cursorHotspotX, m_cursorHotspotY);
    static bool cursorIsSet = false;
    if (cursorIsSet) {
        QGuiApplication::changeOverrideCursor(cursor);
    } else {
        QGuiApplication::setOverrideCursor(cursor);
        cursorIsSet = true;
    }
}

QPointF MotorcarCompositor::toSurface(QWaylandSurface *surface, const QPointF &pos) const
{
    return pos - surface->pos();
}

void MotorcarCompositor::setCursorSurface(QWaylandSurface *surface, int hotspotX, int hotspotY)
{
    if ((m_cursorSurface != surface) && surface)
        connect(surface, SIGNAL(damaged(QRect)), this, SLOT(updateCursor()));

    m_cursorSurface = surface;
    m_cursorHotspotX = hotspotX;
    m_cursorHotspotY = hotspotY;
}


QWaylandSurface *MotorcarCompositor::surfaceAt(const QPointF &point, QPointF *local)
{
    Geometry::Ray ray = m_glData->m_camera->computeRay(point.x(), point.y()).transform(m_glData->m_cameraNode->worldTransform());

    SceneGraphNode::RaySurfaceIntersection *intersection = m_sceneGraphRoot->intersectWithSurfaces(ray);

    if(intersection){
        if (local){
            *local = QPointF(intersection->surfaceLocalCoordinates.x, intersection->surfaceLocalCoordinates.y);
        }

        return intersection->surfaceNode->surface();
    }else{
        //qDebug() << "no intersection found between cursor ray and scene graph";
        return NULL;
    }


}


void MotorcarCompositor::render()
{

    m_glData->m_window->makeCurrent();
 //   m_glData->m_backgroundTexture = m_glData->m_textureCache->bindTexture(QOpenGLContext::currentContext(),m_glData->m_backgroundImage);

//    m_glData->m_textureBlitter->bind();
//    // Draw the background image texture
//    m_glData->m_textureBlitter->drawTexture(m_glData->m_backgroundTexture,
//                                  QRect(QPoint(0, 0), m_glData->m_backgroundImage.size()),
//                                  m_glData->m_window->size(),
//                                  0, false, true);
//    m_glData->m_textureBlitter->release();

    m_glData->calculateVPMatrix();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_sceneGraphRoot->traverse(0, m_glData);

    frameFinished();
    // N.B. Never call glFinish() here as the busylooping with vsync 'feature' of the nvidia binary driver is not desirable.
    m_glData->m_window->swapBuffers();
}

bool MotorcarCompositor::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != m_glData->m_window)
        return false;

    QWaylandInputDevice *input = defaultInputDevice();

    switch (event->type()) {
    case QEvent::Expose:
        m_renderScheduler.start(0);
        if (m_glData->m_window->isExposed()) {
            // Alt-tabbing away normally results in the alt remaining in
            // pressed state in the clients xkb state. Prevent this by sending
            // a release. This is not an issue in a "real" compositor but
            // is very annoying when running in a regular window on xcb.
            Qt::KeyboardModifiers mods = QGuiApplication::queryKeyboardModifiers();
            if (m_modifiers != mods && input->keyboardFocus()) {
                Qt::KeyboardModifiers stuckMods = m_modifiers ^ mods;
                if (stuckMods & Qt::AltModifier)
                    input->sendKeyReleaseEvent(64); // native scancode for left alt
                m_modifiers = mods;
            }
        }
        break;
    case QEvent::MouseButtonPress: {
        QPointF local;
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        QWaylandSurface *targetSurface = surfaceAt(me->localPos(), &local);
        if (m_dragKeyIsPressed && targetSurface) {
            m_draggingWindow = targetSurface;
            m_drag_diff = local;
        } else {
            if (targetSurface && input->keyboardFocus() != targetSurface) {
                input->setKeyboardFocus(targetSurface);
//                m_surfaces.removeOne(targetSurface);
//                m_surfaces.append(targetSurface);
                m_renderScheduler.start(0);
            }
            input->sendMousePressEvent(me->button(), local, me->localPos());
        }
        return true;
    }
    case QEvent::MouseButtonRelease: {
        QWaylandSurface *targetSurface = input->mouseFocus();
        if (m_draggingWindow) {
            m_draggingWindow = 0;
            m_drag_diff = QPointF();
        } else {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            QPointF localPos;
            if (targetSurface)
                localPos = toSurface(targetSurface, me->localPos());
            input->sendMouseReleaseEvent(me->button(), localPos, me->localPos());
        }
        return true;
    }
    case QEvent::MouseMove: {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (m_draggingWindow) {
            m_draggingWindow->setPos(me->localPos() - m_drag_diff);
            m_renderScheduler.start(0);
        } else {
            QPointF local;
            QWaylandSurface *targetSurface = surfaceAt(me->localPos(), &local);
            input->sendMouseMoveEvent(targetSurface, local, me->localPos());
        }
        break;
    }
    case QEvent::Wheel: {
        QWheelEvent *we = static_cast<QWheelEvent *>(event);
        input->sendMouseWheelEvent(we->orientation(), we->delta());
        break;
    }
    case QEvent::KeyPress: {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Meta || ke->key() == Qt::Key_Super_L) {
            m_dragKeyIsPressed = true;
        }else if(ke->key() == Qt::Key_Up){
            m_glData->m_cameraNode->setTransform(glm::translate(glm::mat4(1), glm::vec3(0,0,0.001f)) * m_glData->m_cameraNode->transform());
        }else if(ke->key() == Qt::Key_Down){
            m_glData->m_cameraNode->setTransform(glm::translate(glm::mat4(1), glm::vec3(0,0,-0.001f)) * m_glData->m_cameraNode->transform());
        }
        m_modifiers = ke->modifiers();
        QWaylandSurface *targetSurface = input->keyboardFocus();
        if (targetSurface)
            input->sendKeyPressEvent(ke->nativeScanCode());
        break;
    }
    case QEvent::KeyRelease: {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Meta || ke->key() == Qt::Key_Super_L) {
            m_dragKeyIsPressed = false;
        }
        m_modifiers = ke->modifiers();
        QWaylandSurface *targetSurface = input->keyboardFocus();
        if (targetSurface)
            input->sendKeyReleaseEvent(ke->nativeScanCode());
        break;
    }
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QWaylandSurface *targetSurface = 0;
        QTouchEvent *te = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> points = te->touchPoints();
        QPoint pointPos;
        if (!points.isEmpty()) {
            pointPos = points.at(0).pos().toPoint();
            targetSurface = surfaceAt(pointPos);
        }
        if (targetSurface && targetSurface != input->mouseFocus())
            input->setMouseFocus(targetSurface, pointPos, pointPos);
        if (input->mouseFocus())
            input->sendFullTouchEvent(te);
        break;
    }
    default:
        break;
    }
    return false;
}
