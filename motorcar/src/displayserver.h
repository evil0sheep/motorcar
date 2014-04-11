#ifndef DISPLAYSERVER_H
#define DISPLAYSERVER_H

#include "wayland/output/waylandsurface.h"

namespace motorcar{
///This class handles client connection/disconnection and most of the direct wayland interactions
/*Designed mainly to abstract the qtwayland dependency out of the core scenegraph code and allow the option of
 * moving to a weston plugin as the wayland backend, this class handles all of the EGL, kms, evdev etc nastyness and creates
 * surfaces for use in the scenegraph. It also creates motorcar events for traditional 2D input devices which get passed
 * to the window manager and handled along with */
class DisplayServer
{
public:
    DisplayServer();

};
}


#endif // DISPLAYSERVER_H
