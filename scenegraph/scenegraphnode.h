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

#ifndef SCENEGRAPHNODE_H
#define SCENEGRAPHNODE_H

#include <QObject>
#include <QtDebug>
#include "qwaylandsurface.h"
#include "opengldata.h"
#include "geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


class OpenGLData;
class QtwaylandSurfaceNode;
class DisplayNode;
class Geometry;
class SceneGraphNode : public QObject
{
    Q_OBJECT
public:
    explicit SceneGraphNode(QObject *parent = 0, glm::mat4 transform = glm::mat4(1));

    //calls destructor on all children and removes this node from its parent's list of children
    virtual ~SceneGraphNode();

    //update node transform and other variable with respect to the number of milliseconds elapsed since the last traversal of the scenegraph (deltaTime)
    //returns whether or not animation was successfulOpenGLData *glData
    virtual bool animate(const float deltaTime);
    //draw the contents of this node if necccessary
    //returns whether or not the draw call was successful
    virtual bool draw(DisplayNode *display);
    //traverse the children of this node in the scenegraph
    //left virtual in case it is desireable to handle the failure of child node traversals
    virtual void traverseChildren(float deltaTime, DisplayNode *display);
    //traverses this node in the scenegraph, drawing/animating/updating as neccessary with respect to the number of milliseconds elapsed since the last traversal of the scenegraph (deltaTime)
    bool traverse(float deltaTime, DisplayNode *display);


    //returns this node's transform relative to its parent
    glm::mat4 transform() const;
    //sets this node's transform relative to its parent
    void setTransform(const glm::mat4 &value);
    //returns the inverse of this node's transform relative to its parent
    glm::mat4 inverseTransform() const;
    //returns this node's transform relative to the world
    glm::mat4 worldTransform() const;


    SceneGraphNode *parentNode() const;
    //removes this node from its existing parent's list of children and sets the given node to be this nodes parent
    void setParentNode(SceneGraphNode *parent);
    //adds the given node to the list of children and sets the childs parent to be this node
    void addChildNode(SceneGraphNode *child);
    //removes the given node from the list of children
    void removeChildNode(SceneGraphNode *node);
    //returns whether or not the given SceneGraphNode exists in the SceneGraph subtree rooted at this node
    bool existsInSubtree(SceneGraphNode *node);
    //searches the sceneGraph for WaylandSurfaceNodes by surface, returns null if a furface node with this surface is not found
    //if surface is null returns the first surface node it encounters, or null if none exist in this subtree
    virtual QtwaylandSurfaceNode *getSurfaceNode(const QWaylandSurface *surface = 0);



    struct RaySurfaceIntersection
    {
        RaySurfaceIntersection(QtwaylandSurfaceNode *surfaceNode, QPointF surfaceLocalCoordinates , const Geometry::Ray &ray , float t );
        QtwaylandSurfaceNode *surfaceNode;
        QPointF surfaceLocalCoordinates;
        Geometry::Ray ray;
        float t;
    };

    //returns the intersection of the given parent space ray with the closest surface in the scenegraph subtree rooted at this node or NULL if no intersection is found
    virtual SceneGraphNode::RaySurfaceIntersection *intersectWithSurfaces(const Geometry::Ray &ray);







protected:
    glm::mat4 m_transform, m_inverseTransform;
    SceneGraphNode *m_parentNode;
    QList<SceneGraphNode *> m_childNodes;


signals:
    
public slots:
    
};

#endif // SCENEGRAPHNODE_H
