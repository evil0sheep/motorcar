#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "outputelement.h"
#include "../virtualnode.h"
#include "glcameranode.h"

namespace motorcar {
class Drawable : public OutputElement, public VirtualNode
{
public:
    Drawable(SceneGraphNode &parent, const glm::mat4 &transform = glm::mat4());
    virtual ~Drawable();

    //calls VirtualNode::traverseNode() and then draw()
    virtual void traverseNode(long deltaMillis);



    //draws this node from the specified viewpoint
    //implementations typically get the display associated with this viewpoint and make a viewpoint specific draw call to that display
    //this exists to allow the drawable to control what content is sent to which viewpoints
    virtual void drawViewpoint(GLCamera *viewpoint) = 0;

private:
    //draws this node to every display owned by users with read permissions on this node
    //this allows the drawable to control which displays it's drawn to
    void draw();
};
}


#endif // DRAWABLE_H
