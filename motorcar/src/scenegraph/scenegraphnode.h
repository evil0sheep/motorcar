#ifndef SCENEGRAPHNODE_H
#define SCENEGRAPHNODE_H
#include <glm/glm.hpp>
#include <type_traits>
#include <vector>
#include <algorithm>
#include "../geometry.h"

//#include "output/wayland/waylandsurfacenode.h"

#include "foo.h"


namespace motorcar {

class Scene;


class SceneGraphNode : public Foo
{
public:
    SceneGraphNode(SceneGraphNode *parent, glm::mat4 transform = glm::mat4());
    ///calls destructor on all children and removes this node from its parent's list of children
    virtual ~SceneGraphNode();

    ///handles any additional action this node wishes to take as it is traversed over (e.g. drawing, updating etc)
    ///default behavior does nothing unless overriddedn
    virtual void traverseNode(Scene *scene, long deltaMillis);
    ///traverses this node and then traverses all of its children
    void traverseSceneGraph(Scene *scene, long deltaMillis);

    ///gets this node's parent in the scenegraph
    SceneGraphNode *parentNode() const;
    ///gets the scene which forms the root of the scenegraph this node is embedded in
    ///returns NULL if this node is not rooted in a scene
    Scene *scene();
    ///returns whether or not the given Node exists in the subtree rooted at this node
    bool subtreeContains(SceneGraphNode *node);


    ///returns this node's transform relative to its parent
    glm::mat4 transform() const;
    ///returns the inverse of this node's transform relative to its parent
    ///(inverse transform is cached whenever transform is set)
    glm::mat4 inverseTransform() const;
    ///sets this node's transform relative to its parent
    virtual void setTransform(const glm::mat4 &transform);

    ///returns this node's transform relative to the world
    glm::mat4 worldTransform() const;
    ///returns inverse transform relative to the world (computed on demand)
    glm::mat4 inverseWorldTransform() const;
    ///sets this node's transform relative to the world
    void setWorldTransform(const glm::mat4 &transform);




    ///returns the intersection of the given parent space ray with the closest surface in the scenegraph subtree rooted at this node or NULL if no intersection is found
    virtual Geometry::RaySurfaceIntersection *intersectWithSurfaces(const Geometry::Ray &ray);



    ///returns the immediate children of this node
    std::vector<SceneGraphNode *> childNodes() const;

    ///returns a list of all nodes in this the scenegraph subtree rooted at this node
    std::vector<SceneGraphNode *> nodesInSubtree() const;


private :
    //traverse the children of this node in the scenegraph
    void traverseChildren(Scene *scene, long deltaMillis);

    glm::mat4 m_transform, m_inverseTransform;
    SceneGraphNode *m_parentNode;
    std::vector<SceneGraphNode *> m_childNodes;

    //adds the given node to the list of children
    //if child is NULL this call will be ignored
    void addChildNode(SceneGraphNode *child);
    //removes the given node from the list of children if it exists therein
    void removeChildNode(SceneGraphNode *node);

protected:
    //protected constructor used by Scene class to construct parentless, un-transformed root node
    SceneGraphNode();
    //removes this node from its existing parent's list of children and sets the given node to be this nodes parent and then adds this node to the given node's list of children
    void setParentNode(SceneGraphNode *parent);

};

}


#endif // SCENEGRAPHNODE_H
