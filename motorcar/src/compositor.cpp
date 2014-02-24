#include "compositor.h"

using namespace motorcar;


Compositor::~Compositor()
{
    delete m_display;
}

Display *Compositor::display() const
{
    return m_display;
}

void Compositor::setDisplay(Display *display)
{
    m_display = display;
}
