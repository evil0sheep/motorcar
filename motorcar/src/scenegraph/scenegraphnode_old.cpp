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

#include "scenegraphnode_old.h"

SceneGraphNode::SceneGraphNode(QObject *parent, glm::mat4 transform) :
    QObject(parent)
  , m_parentNode(NULL)

{ 
    setTransform(transform);
    if(dynamic_cast<SceneGraphNode*>(parent) != NULL){
        SceneGraphNode *parentNode = dynamic_cast<SceneGraphNode*>(parent);
        setParentNode(parentNode);
        parentNode->addChildNode(this);
    }
    
}

SceneGraphNode::~SceneGraphNode(){
    if (this->parentNode() != NULL)
        this->parentNode()->removeChildNode(this);

    foreach (SceneGraphNode *child, m_childNodes) {
        delete child;
    }
}

bool SceneGraphNode::animate(const float deltaTime)
{
    return true;
}

bool SceneGraphNode::draw(DisplayNode *display)
{
    return true;
}


void SceneGraphNode::traverseChildren(float deltaTime, DisplayNode *display){
    foreach (SceneGraphNode *child, m_childNodes) {
        if (child != NULL){
            child->traverse(deltaTime, display);
        }
    }

}

bool SceneGraphNode::traverse(float deltaTime, DisplayNode *display){

    if(!this->animate(deltaTime) || !this->draw(display)){
        return false;
    }
    this->traverseChildren(deltaTime, display);
    return true;
}




void SceneGraphNode::setParentNode(SceneGraphNode *parent)
{
    if (this->parentNode() != NULL){
        this->parentNode()->removeChildNode(this);
    }
    this->m_parentNode = parent;
}

void SceneGraphNode::addChildNode(SceneGraphNode *child)
{
    child->setParent(this);
    this->m_childNodes.append(child);

}

void SceneGraphNode::removeChildNode(SceneGraphNode *node)
{
    if(!m_childNodes.isEmpty()){
        m_childNodes.removeOne(node);
    }
}

bool SceneGraphNode::existsInSubtree(SceneGraphNode *node)
{
    if(node == this) return true;
    foreach (SceneGraphNode *child, m_childNodes) {
        if (child != NULL && child->existsInSubtree(node)){
             return true;

        }
    }
    return false;

}

QtwaylandSurfaceNode *SceneGraphNode::getSurfaceNode(const QWaylandSurface *surface)
{
    foreach (SceneGraphNode *child, m_childNodes) {
        if (child != NULL){
            QtwaylandSurfaceNode *node = child->getSurfaceNode(surface);
            if(node) return node;
        }
    }
    return NULL;

}

SceneGraphNode::RaySurfaceIntersection *SceneGraphNode::intersectWithSurfaces(const Geometry::Ray &ray)
{
    SceneGraphNode::RaySurfaceIntersection *closestIntersection = NULL, *currentIntersection;
    Geometry::Ray transformedRay = ray.transform(inverseTransform());
    foreach (SceneGraphNode *child, m_childNodes) {
        if (child != NULL){
            currentIntersection = child->intersectWithSurfaces(transformedRay);
            if(closestIntersection == NULL || (currentIntersection != NULL && currentIntersection->t < closestIntersection->t)){
                delete closestIntersection;
                closestIntersection = currentIntersection;
            }
        }
    }
    return closestIntersection;
}

glm::mat4 SceneGraphNode::worldTransform() const
{
    if(this->m_parentNode != NULL){
        return this->parentNode()->worldTransform() * this->transform();
    }else{
        return this->transform();
    }
}


SceneGraphNode *SceneGraphNode::parentNode() const
{
    return this->m_parentNode;
}


glm::mat4 SceneGraphNode::transform() const
{
    return m_transform;
}

void SceneGraphNode::setTransform(const glm::mat4 &value)
{
    m_transform = value;
    m_inverseTransform = glm::inverse(m_transform);
}

SceneGraphNode::RaySurfaceIntersection::RaySurfaceIntersection(QtwaylandSurfaceNode *surfaceNode, QPointF surfaceLocalCoordinates, const Geometry::Ray &ray, float t)
    : surfaceNode(surfaceNode)
    , surfaceLocalCoordinates(surfaceLocalCoordinates)
    , ray(ray)
    , t(t)
{
}



glm::mat4 SceneGraphNode::inverseTransform() const
{
    return m_inverseTransform;
}


