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
#include <QOpenGLFunctions>
#include <QGuiApplication>
#include <QCursor>
#include <QPixmap>
#include <QLinkedList>
#include <QScreen>
#include <QPainter>

#include <QtCompositor/qwaylandinput.h>

MotorcarCompositor::MotorcarCompositor(QOpenGLWindow *window)
    : QWaylandCompositor(window, 0, DefaultExtensions | SubSurfaceExtension)
    , m_window(window)
    , m_textureBlitter(0)
    , m_renderScheduler(this)
    , m_draggingWindow(0)
    , m_dragKeyIsPressed(false)
    , m_cursorSurface(0)
    , m_cursorHotspotX(0)
    , m_cursorHotspotY(0)
    , m_modifiers(Qt::NoModifier)
{
    m_window->makeCurrent();

    m_textureCache = new QOpenGLTextureCache(m_window->context());
    m_textureBlitter = new TextureBlitter();
    m_backgroundImage = makeBackgroundImage(QLatin1String(":/background.jpg"));
    m_renderScheduler.setSingleShot(true);
    connect(&m_renderScheduler,SIGNAL(timeout()),this,SLOT(render()));

    QOpenGLFunctions *functions = m_window->context()->functions();
    functions->glGenFramebuffers(1, &m_surface_fbo);

    window->installEventFilter(this);

    setRetainedSelectionEnabled(true);

    setOutputGeometry(QRect(QPoint(0, 0), window->size()));
    setOutputRefreshRate(qRound(qGuiApp->primaryScreen()->refreshRate() * 1000.0));
}

MotorcarCompositor::~MotorcarCompositor()
{
    delete m_textureBlitter;
    delete m_textureCache;
}


QImage MotorcarCompositor::makeBackgroundImage(const QString &fileName)
{
    Q_ASSERT(m_window);

    int width = m_window->width();
    int height = m_window->height();
    QImage baseImage(fileName);
    QImage patternedBackground(width, height, baseImage.format());
    QPainter painter(&patternedBackground);

    QSize imageSize = baseImage.size();
    for (int y = 0; y < height; y += imageSize.height()) {
        for (int x = 0; x < width; x += imageSize.width()) {
            painter.drawImage(x, y, baseImage);
        }
    }

    return patternedBackground;
}

void MotorcarCompositor::ensureKeyboardFocusSurface(QWaylandSurface *oldSurface)
{
    QWaylandSurface *kbdFocus = defaultInputDevice()->keyboardFocus();
    if (kbdFocus == oldSurface || !kbdFocus)
        defaultInputDevice()->setKeyboardFocus(m_surfaces.isEmpty() ? 0 : m_surfaces.last());
}

void MotorcarCompositor::surfaceDestroyed(QObject *object)
{
    QWaylandSurface *surface = static_cast<QWaylandSurface *>(object);
    m_surfaces.removeOne(surface);
    ensureKeyboardFocusSurface(surface);
    m_renderScheduler.start(0);
}

void MotorcarCompositor::surfaceMapped()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    QPoint pos;
    if (!m_surfaces.contains(surface)) {
        uint px = 0;
        uint py = 0;
        if (!QCoreApplication::arguments().contains(QLatin1String("-stickytopleft"))) {
            px = 1 + (qrand() % (m_window->width() - surface->size().width() - 2));
            py = 1 + (qrand() % (m_window->height() - surface->size().height() - 2));
        }
        pos = QPoint(px, py);
        surface->setPos(pos);
    } else {
        m_surfaces.removeOne(surface);
    }
    //Sometimes surfaces dont have shell_surfaces, so don't render them
    if (surface->hasShellSurface()) {
        m_surfaces.append(surface);
        defaultInputDevice()->setKeyboardFocus(surface);
    }
    m_renderScheduler.start(0);
}

void MotorcarCompositor::surfaceUnmapped()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    if (m_surfaces.removeOne(surface))
        m_surfaces.insert(0, surface);

    ensureKeyboardFocusSurface(surface);
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
    for (int i = m_surfaces.size() - 1; i >= 0; --i) {
        QWaylandSurface *surface = m_surfaces.at(i);
        QRectF geo(surface->pos(), surface->size());
        if (geo.contains(point)) {
            if (local)
                *local = toSurface(surface, point);
            return surface;
        }
    }
    return 0;
}

GLuint MotorcarCompositor::composeSurface(QWaylandSurface *surface)
{
    GLuint texture = 0;

    QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();
    functions->glBindFramebuffer(GL_FRAMEBUFFER, m_surface_fbo);

    if (surface->type() == QWaylandSurface::Shm) {
        texture = m_textureCache->bindTexture(QOpenGLContext::currentContext(),surface->image());
    } else if (surface->type() == QWaylandSurface::Texture) {
        texture = surface->texture(QOpenGLContext::currentContext());
    }

    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                       GL_TEXTURE_2D, texture, 0);
    paintChildren(surface,surface);
    functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                       GL_TEXTURE_2D,0, 0);

    functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return texture;
}

void MotorcarCompositor::paintChildren(QWaylandSurface *surface, QWaylandSurface *window) {

    if (surface->subSurfaces().size() == 0)
        return;

    QLinkedListIterator<QWaylandSurface *> i(surface->subSurfaces());
    while (i.hasNext()) {
        QWaylandSurface *subSurface = i.next();
        QPointF p = subSurface->mapTo(window,QPointF(0,0));
        if (subSurface->size().isValid()) {
            GLuint texture = 0;
            if (subSurface->type() == QWaylandSurface::Texture) {
                texture = subSurface->texture(QOpenGLContext::currentContext());
            } else if (surface->type() == QWaylandSurface::Shm ) {
                texture = m_textureCache->bindTexture(QOpenGLContext::currentContext(),surface->image());
            }
            QRect geo(p.toPoint(),subSurface->size());
            m_textureBlitter->drawTexture(texture,geo,window->size(),0,window->isYInverted(),subSurface->isYInverted());
        }
        paintChildren(subSurface,window);
    }
}


void MotorcarCompositor::render()
{
    m_window->makeCurrent();
    m_backgroundTexture = m_textureCache->bindTexture(QOpenGLContext::currentContext(),m_backgroundImage);

    m_textureBlitter->bind();
    // Draw the background image texture
    m_textureBlitter->drawTexture(m_backgroundTexture,
                                  QRect(QPoint(0, 0), m_backgroundImage.size()),
                                  window()->size(),
                                  0, false, true);

    foreach (QWaylandSurface *surface, m_surfaces) {
        if (!surface->visible())
            continue;
        GLuint texture = composeSurface(surface);
        QRect geo(surface->pos().toPoint(),surface->size());
        m_textureBlitter->drawTexture(texture,geo,m_window->size(),0,false,surface->isYInverted());
    }

    m_textureBlitter->release();
    frameFinished();
    // N.B. Never call glFinish() here as the busylooping with vsync 'feature' of the nvidia binary driver is not desirable.
    m_window->swapBuffers();
}

bool MotorcarCompositor::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != m_window)
        return false;

    QWaylandInputDevice *input = defaultInputDevice();

    switch (event->type()) {
    case QEvent::Expose:
        m_renderScheduler.start(0);
        if (m_window->isExposed()) {
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
                m_surfaces.removeOne(targetSurface);
                m_surfaces.append(targetSurface);
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
