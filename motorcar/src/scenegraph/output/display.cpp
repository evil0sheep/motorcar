#include "display.h"

using namespace motorcar;

Display::Display()
{
}

Display::Display(std::vector<motorcar::GLCamera *> viewpoints)
    :m_viewpoints(viewpoints)
{

}

Display::~Display()
{
}

void Display::prepare()
{
    for(GLCamera *viewpoint : viewpoints()){
        viewpoint->calculateVPMatrix();
    }
}

void Display::renderDrawable(Drawable *drawable)
{
    for(GLCamera *viewpoint : m_viewpoints){
        drawable->drawViewpoint(viewpoint);
    }
}

void Display::addViewpoint(GLCamera *v)
{
    m_viewpoints.push_back(v);
}







std::vector<GLCamera *> Display::viewpoints() const
{
    return m_viewpoints;
}


