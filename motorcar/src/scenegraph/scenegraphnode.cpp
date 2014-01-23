#include "scenegraphnode.h"

using namespace motorcar;


SceneGraphNode::SceneGraphNode(SceneGraphNode &parent, glm::mat4 transform)
{
    this->setParentNode(parent);
    this->setTransform(transform);
}


SceneGraphNode::SceneGraphNode(SceneGraphNode *parent)
{
}



SceneGraphNode::~SceneGraphNode(){
    if (this->parentNode() != NULL)
        this->parentNode()->removeChildNode(this);

    foreach (SceneGraphNode *child, m_childNodes) {
        delete child;
    }
}

void SceneGraphNode::traverseNode(long deltaMillis)
{
}

void SceneGraphNode::traverseSceneGraph(long deltaMillis)
{
    traverseNode(deltaMillis);
    traverseChildren(deltaMillis);
}




void SceneGraphNode::setParentNode(SceneGraphNode &parent)
{
    if (this->parentNode() != NULL){
        this->parentNode()->removeChildNode(this);
    }
    parent.addChildNode(this);
    this->m_parentNode = std::addressof(parent);

}

void SceneGraphNode::addChildNode(SceneGraphNode *child)
{
    if(child != NULL){
        this->m_childNodes.append(child);
    }

}

void SceneGraphNode::removeChildNode(SceneGraphNode *node)
{
    if(!m_childNodes.isEmpty()){
        m_childNodes.removeOne(node);
    }
}





void SceneGraphNode::traverseChildren(long deltaMillis)
{
    for (SceneGraphNode *child : m_childNodes) {
        if (child != NULL){
            child->traverse(deltaMillis);
        }
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
}

void SceneGraphNode::setWorldTransform(const glm::mat4 &transform)
{
    setWorldTransform(parentNode()->inverseWorldTransform() * transform);

}


WaylandSurfaceNode::RaySurfaceIntersection *SceneGraphNode::intersectWithSurfaces(const Geometry::Ray &ray)
{
    WaylandSurfaceNode::RaySurfaceIntersection *closestIntersection = NULL, *currentIntersection;
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





