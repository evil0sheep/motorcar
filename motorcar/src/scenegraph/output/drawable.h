#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "../virtualnode.h"
#include "viewpoint.h"

namespace motorcar {
class Drawable : public VirtualNode
{
public:
    Drawable(SceneGraphNode *parent, const glm::mat4 &transform = glm::mat4());
    virtual ~Drawable(){}

    ///Draw this node for the current display
    /* this method should contain all the openGL code needed to draw the node,
     * implementations can assume that the correct context is current and the
     * correct framebuffer is bound. If the framebuffer needs to be unbound
     * the implementation must tell the display to rebind its framebuffer
     * before drawing into it, do not assume drawing into the default
     * framebuffer is safe*/
    virtual void draw(Scene *scene, Display *display) = 0;

    ///Gets the active display from the scene and calls draw on it
    virtual void handleFrameDraw(Scene *scene) override;

    bool visible() const;
    void setVisible(bool visible);

private:
    bool m_visible;

};
}


#endif // DRAWABLE_H
