#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "../geometry.h"
#include <GLES2/gl2.h>
namespace motorcar{

//normalized viewport within a given window,
//all numerical arguments range from 0 to 1 and are multiplied by the size of the window in all getters and before being passed to OpenGL calls
class ViewPort : public Geometry::Rectangle
{
public:

    ViewPort(glm::vec2 normalizedPos, glm::vec2 normalizedSize, Rectangle *bufferGeometry);
    ~ViewPort() {}

    float offsetX() const;
    float offsetY() const;
    float width() const;
    float height() const;


    //returns resolution of this viewport in pixels, inherited from rectangle
    virtual glm::ivec2 size() override;

    //calls glViewport with parameters
    void set() const;


    glm::vec2 displayCoordsToViewportCoords(float pixelX, float pixelY) const;

    //takes a buffer of at least 8 floats and fills it with the UV space texture coordinates for the four corners of the viewport
    void uvCoords(float *buf);

    //returns a vector of the four floats which this was constructed on
    glm::vec4 viewportParams() const;



    Rectangle *bufferGeometry() const;
    void setBufferGeometry(Rectangle *bufferGeometry);

private:
    Rectangle *m_bufferGeometry;
    float m_offsetX, m_offsetY, m_width, m_height;

};
}



#endif // VIEWPORT_H
