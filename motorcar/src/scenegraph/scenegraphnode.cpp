#include "scenegraphnode.h"

using namespace motorcar;


SceneGraphNode::SceneGraphNode(SceneGraphNode &parent, glm::mat4 transform)
{
    this->setParentNode(parent);
    this->setTransform(transform);
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

    Scene *scene = this->scene();
    if(!scene->subtreeContains(this)){
        scene->notifyNodeAdded(this);
    }

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

Scene *SceneGraphNode::scene() const
{
    if(m_parentNode != NULL){
        //if we are not at the root of the scenegraph yet we keep going up
        return m_parentNode->scene();
    }else{
        //we cast ourselves to a Scene and return (this returns NULL if the root is not a Scene
        //which is consistent with the function's defined semantics)
        return dynamic_cast<Scene *>(this);
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





