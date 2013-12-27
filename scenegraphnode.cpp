#include "scenegraphnode.h"

SceneGraphNode::SceneGraphNode(QObject *parent) :
    QObject(parent)
{ 
    if(dynamic_cast<SceneGraphNode*>(parent) != NULL){
        SceneGraphNode *parentNode = dynamic_cast<SceneGraphNode*>(parent);
        setParentNode(parentNode);
        parentNode->addChildNode(this);
    }
    
}

SceneGraphNode::~SceneGraphNode(){
    foreach (SceneGraphNode *child, mChildren) {
        delete child;
    }
}


void SceneGraphNode::traverseChildren(float deltaTime){
    foreach (SceneGraphNode *child, mChildren) {
        child->traverse(deltaTime);
    }

}

bool SceneGraphNode::traverse(float deltaTime){

    if(!this->animate(deltaTime) || !this->draw()){
        return false;
    }
    this->traverseChildren(deltaTime);
    return true;
}


void SceneGraphNode::setParentNode(SceneGraphNode *parent)
{
    this->mParentNode = parent;
}

void SceneGraphNode::addChildNode(SceneGraphNode *child)
{
    this->mChildren.append(child);
    child->setParent(this);
}

glm::mat4 SceneGraphNode::getWorldTransform()
{
    if(this->mParentNode != NULL){
        return this->getParentNode()->getWorldTransform() * this->mTransform;
    }else{
        return this->mTransform;
    }
}


SceneGraphNode *SceneGraphNode::getParentNode() const
{
    return this->mParentNode;
}

