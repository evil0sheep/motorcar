#ifndef WAYLANDDRAWABLE_H
#define WAYLANDDRAWABLE_H

#include "../drawable.h"
namespace motorcar {
    class WaylandDrawable : public Drawable
    {
    public:
        WaylandDrawable(SceneGraphNode &parent, const glm::mat4 &transform = glm::mat4(1));
        virtual ~WaylandDrawable();
    };
}


#endif // WAYLANDDRAWABLE_H
