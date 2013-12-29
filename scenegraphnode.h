#ifndef SCENEGRAPHNODE_H
#define SCENEGRAPHNODE_H

#include <QObject>
#include <QtDebug>
#include "qwaylandsurface.h"
#include "opengldata.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


class MotorcarSurfaceNode;
class SceneGraphNode : public QObject
{
    Q_OBJECT
public:
    explicit SceneGraphNode(QObject *parent = 0);

    //calls destructor on all children and removes this node from its parent's list of children
    virtual ~SceneGraphNode();

    //update node transform and other variable with respect to the number of milliseconds elapsed since the last traversal of the scenegraph (deltaTime)
    //returns whether or not animation was successful
    virtual bool animate(const float deltaTime);
    //draw the contents of this node if necccessary
    //returns whether or not the draw call was successful
    virtual bool draw(OpenGLData *glData);
    //traverse the children of this node in the scenegraph
    //left virtual in case it is desireable to handle the failure of child node traversals
    virtual void traverseChildren(float deltaTime, OpenGLData *glData);

    //traverses this node in the scenegraph, drawing/animating/updating as neccessary with respect to the number of milliseconds elapsed since the last traversal of the scenegraph (deltaTime)
    bool traverse(float deltaTime, OpenGLData *glData);


    //returns this node's transform relative to its parent
    glm::mat4 transform() const;
    //sets this node's transform relative to its parent
    void setTransform(const glm::mat4 &value);
    //returns this node's transform relative to the world
    glm::mat4 worldTransform();


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
    virtual MotorcarSurfaceNode *getSurfaceNode(const QWaylandSurface *surface = 0);


protected:
    glm::mat4 m_transform;
    SceneGraphNode *m_parentNode;
    QList<SceneGraphNode *> m_childNodes;


signals:
    
public slots:
    
};

#endif // SCENEGRAPHNODE_H
