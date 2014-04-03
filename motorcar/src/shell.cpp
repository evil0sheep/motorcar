#include "shell.h"
#include "scenegraph/scene.h"
#include "compositor.h"

using namespace motorcar;

void make_motorcar_surface(struct wl_client *client,
                           struct wl_resource *resource,
                           struct wl_resource *surface)
{
    std::cout << "OMG PROTOCOL EXTENSIONS BITCHES" <<std::endl;
}


const static struct motorcar_shell_interface motorcarShellInterface = {
    make_motorcar_surface
};

Shell::Shell(Scene *scene)
{
    m_display = scene->compositor()->wlDisplay();
    //wl_display_interface.get_registry;
    //struct wl_global* wl_global_create(display, const struct wl_interface *interface, int version, void *data, wl_global_bind_func_t bind);
    struct wl_global *global =0;

    global = wl_global_create(m_display,
                     &motorcar_shell_interface,
                      motorcar_shell_interface.version,
                     this,
                     Shell::bind_func);

    std::cout << "creating global shell object" <<std::endl;
    //struct motorcar_shell_interface *shell =
}

void Shell::bind_func(struct wl_client *client, void *data,
                      uint32_t version, uint32_t id)
{
    struct wl_resource *resource = wl_resource_create(client, &motorcar_shell_interface, version, id);
    wl_resource_set_implementation(resource, &motorcarShellInterface, data, 0);
}
