/****************************************************************************
**This file is part of the Motorcar 3D windowing framework
**
**
**Copyright (C) 2014 Forrest Reiling
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
#ifndef SCENE_H
#define SCENE_H

#include "physicalnode.h"
#include "output/display/display.h"

namespace motorcar {
class WindowManager;
class Compositor;
class Scene : public PhysicalNode
{
public:

    Scene();
    virtual ~Scene();




    ///Overloads SceneGraphNode definition to return this node
    Scene *scene() override;

    void prepareForFrame(long timeStampMillis);
    void drawFrame();
    void finishFrame();


    WindowManager *windowManager() const;
    void setWindowManager(WindowManager *windowManager);

    Compositor *compositor() const;
    void setCompositor(Compositor *compositor);

    void addDisplay(Display *display);
    std::vector<Display *> displays() const;

    Display *activeDisplay() const;
    void setActiveDisplay(Display *activeDisplay);


    long currentTimestampMillis() const;
    void setCurrentTimestampMillis(long currentTimestampMillis);

    long latestTimestampChange();



private:
    long m_currentTimestampMillis, m_lastTimestepMillis;
    WindowManager *m_windowManager;
    Compositor *m_compositor;
    Scene *m_trash;

    std::vector<Display *> m_displays;
    Display *m_activeDisplay;

};
}


#endif // SCENE_H
