#include "waylanddrawable.h"

using namespace motorcar;
WaylandDrawable::WaylandDrawable(SceneGraphNode &parent, const glm::mat4 &transform)
    :Drawable(parent, transform)
{
}
