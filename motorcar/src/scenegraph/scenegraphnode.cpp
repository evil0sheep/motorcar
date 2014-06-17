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
#include "scenegraphnode.h"
#include "scene.h"

using namespace motorcar;


SceneGraphNode::SceneGraphNode(SceneGraphNode *parent, glm::mat4 transform)
    :m_parentNode(NULL)
{
    this->setParentNode(parent);
    this->setTransform(transform);
}




bool SceneGraphNode::subtreeContains(SceneGraphNode *node)
{
    if(node == this) return true;
    for (SceneGraphNode *child : m_childNodes) {
        if (child != NULL && child->subtreeContains(node)){
             return true;

        }
    }
    return false;

}


SceneGraphNode::~SceneGraphNode(){

    if (this->parentNode() != NULL)
        this->parentNode()->removeChildNode(this);

    for (SceneGraphNode *child : m_childNodes) {
        if(!child->isSurfaceNode() || parentNode() == NULL){
            delete child;
        }else{
            child->setParentNode(scene());
        }
    }
}




void SceneGraphNode::setParentNode(SceneGraphNode *parent)
{
    if(parent == NULL){
        std::cout << "ERROR: Setting SceneGraphNode parent to NULL, behavior is undefined" << std::endl;
    }

    if (this->parentNode() != NULL ){
        this->parentNode()->removeChildNode(this);
    }



    this->m_parentNode = parent;

    if(parent != NULL){
        parent->addChildNode(this);

    }




}

void SceneGraphNode::addChildNode(SceneGraphNode *child)
{
    if(child != NULL){
        this->m_childNodes.push_back(child);
    }

}

void SceneGraphNode::removeChildNode(SceneGraphNode *node)
{

    std::vector<SceneGraphNode *>::iterator position = std::find(m_childNodes.begin(), m_childNodes.end(), node);
    if (position != m_childNodes.end()){
         m_childNodes.erase(position);
    }

}

void SceneGraphNode::mapOntoSubTree(void (SceneGraphNode::*mapFunc)(Scene *), Scene * scene)
{
    ((this)->*(mapFunc))(scene);
    for(SceneGraphNode *childNode : m_childNodes){
        ((childNode)->mapOntoSubTree(mapFunc, scene));
    }
}

SceneGraphNode::SceneGraphNode()
{
    m_parentNode = NULL;
}





SceneGraphNode *SceneGraphNode::parentNode() const
{
    return this->m_parentNode;
}

Scene *SceneGraphNode::scene()
{
    if(parentNode() != NULL){
        return parentNode()->scene();
    }else{
        return NULL;
    }

}


glm::mat4 SceneGraphNode::transform() const
{
    return m_transform;
}

glm::mat4 SceneGraphNode::inverseTransform() const
{
    return m_inverseTransform;
}

glm::mat4 SceneGraphNode::worldTransform() const
{
    if(this->m_parentNode != NULL){
        return this->parentNode()->worldTransform() * this->transform();
    }else{
        return this->transform();
    }
}

glm::mat4 SceneGraphNode::inverseWorldTransform() const
{
    return glm::inverse(this->worldTransform());
}

void SceneGraphNode::setTransform(const glm::mat4 &transform)
{
    m_transform = transform;
    m_inverseTransform = glm::inverse(m_transform);
    this->mapOntoSubTree(&SceneGraphNode::handleWorldTransformChange,this->scene());
}

void SceneGraphNode::setWorldTransform(const glm::mat4 &transform)
{
    setTransform(parentNode()->inverseWorldTransform() * transform);

}


Geometry::RaySurfaceIntersection *SceneGraphNode::intersectWithSurfaces(const Geometry::Ray &ray)
{
    Geometry::RaySurfaceIntersection *closestIntersection = NULL, *currentIntersection;
    Geometry::Ray transformedRay = ray.transform(inverseTransform());
    for (SceneGraphNode *child : m_childNodes) {
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






std::vector<SceneGraphNode *> SceneGraphNode::childNodes() const
{
    return m_childNodes;
}

std::vector<SceneGraphNode *> SceneGraphNode::nodesInSubtree() const
{
    std::vector<SceneGraphNode *> results;
    results.insert(results.end(), m_childNodes.begin(), m_childNodes.end());
    for(SceneGraphNode *child : m_childNodes){
        std::vector<SceneGraphNode *> childNodes = child->nodesInSubtree();
        results.insert(results.end(), childNodes.begin(), childNodes.end());
    }
    return results;
}


