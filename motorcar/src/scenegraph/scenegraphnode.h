#ifndef SCENEGRAPHNODE_H
#define SCENEGRAPHNODE_H
#include <glm/glm.hpp>
#include <type_traits>
#include <vector>
#include <algorithm>
#include "../geometry.h"

//#include "output/wayland/waylandsurfacenode.h"




namespace motorcar {

class Scene;


class SceneGraphNode
{
public:
    SceneGraphNode(SceneGraphNode *parent, glm::mat4 transform = glm::mat4());
    ///calls destructor on all children and removes this node from its parent's list of children
    virtual ~SceneGraphNode();

    ///Set up the current node for the next frame
    /* This is virtual function called once per frame on all nodes in the scenegraph,
     * should be overridden by classes that need to do per-frame setup work, including animation or state updates
     * this is the only place where it is safe to bind a different framebuffer without rebinding it to the current display's active framebuffer
     * the spcacial configuration of the scene should not be modified outside of this function*/
    virtual void handleFrameBegin(Scene *scene){}
    ///draw the current node for the next frame
    /* This function is called on ever node in the scenegraph once per display, and should be used to do drawing work for nodes that are drawable.
     * The current active display can be accessed through the Scene and the node should draw content for every viewpoint in this display which
     * it would like to be visible from (typically all of them)*/
    virtual void handleFrameDraw(Scene *scene){}
    ///cleanup after the current frame is finished
    /* this function is called once per frame on every node in the scenegraph, it should be used to clean up graphics resources to be ready for the
     * next frame. If a display does a second rendering pass it should be applied here*/
    virtual void handleFrameEnd(Scene *scene){}


    ///gets this node's parent in the scenegraph
    SceneGraphNode *parentNode() const;
    ///gets the scene which forms the root of the scenegraph this node is embedded in
    ///returns NULL if this node is not rooted in a scene
    virtual Scene *scene();
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

    virtual bool isSurfaceNode(){return false;}

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
    ///protected constructor used by Scene class to construct parentless, un-transformed root node
    SceneGraphNode();
    ///removes this node from its existing parent's list of children and sets the given node to be this nodes parent and then adds this node to the given node's list of children
    void setParentNode(SceneGraphNode *parent);

    ///Maps a given function onto all nodes in the subtree rooted at this node
    /* This function forms the core of the scenegraph, use for all of the pre frame callbacks.
        It takes a function, mapFunc, calls it on the current node, and then recursively maps it onto all of its children*/
    void mapOntoSubTree(void (SceneGraphNode::*mapFunc)(Scene *), Scene *scene);

};

}


#endif // SCENEGRAPHNODE_H
