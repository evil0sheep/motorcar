#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H
#include <glm/glm.hpp>

class AbstractNode
{
public:
    AbstractNode();
    //calls destructor on all children and removes this node from its parent's list of children
    virtual ~AbstractNode();

    //handles any additional action this node wishes to take as it is traversed over (e.g. drawing, updating etc)
    virtual void traverseNode(long deltaMillis);
    //traverses this node in the scenegraph, drawing/animating/updating as neccessary with respect to the number of milliseconds elapsed since the last traversal of the scenegraph (deltaMillis)
    void traverseSceneGraph(long deltaMillis);

    //returns this node's transform relative to its parent
    glm::mat4 transform() const;
    //sets this node's transform relative to its parent
    void setTransform(const glm::mat4 &value);
    //returns the inverse of this node's transform relative to its parent
    //(inverse transform is cached whenever transform is set
    glm::mat4 inverseTransform() const;
    //returns this node's transform relative to the world
    glm::mat4 worldTransform() const;

private :
    //traverse the children of this node in the scenegraph
    void traverseChildren(float deltaTime);

};

#endif // ABSTRACTNODE_H
