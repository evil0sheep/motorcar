#include "shell.h"
#include "scenegraph/scene.h"
#include "compositor.h"
#include "windowmanager.h"

using namespace motorcar;

void get_motorcar_surface(struct wl_client *client,
                           struct wl_resource *resource,
                           uint32_t id,
                           struct wl_resource *surface_resource)
{
    Shell *shell = static_cast<Shell*>(resource->data);

    WaylandSurface *surface = shell->scene()->compositor()->getSurfaceFromResource(surface_resource);

    shell->scene()->windowManager()->mapSurface(surface, WaylandSurface::SurfaceType::DEPTH_COMPOSITED);
    //new ShellSurface(shell, client, id, surface);
}


const static struct motorcar_shell_interface motorcarShellInterface = {
    get_motorcar_surface
};

Shell::Shell(Scene *scene)
    :m_scene(scene)
{
    m_display = scene->compositor()->wlDisplay();

    struct wl_global *global =0;

    global = wl_global_create(m_display,
                     &motorcar_shell_interface,
                      motorcar_shell_interface.version,
                     this,
                     Shell::bind_func);

    std::cout << "creating global shell object" <<std::endl;
    //struct motorcar_shell_interface *shell =
}

Shell::~Shell()
{
    //todo: destroy the shell global
}
Scene *Shell::scene() const
{
    return m_scene;
}





void Shell::bind_func(struct wl_client *client, void *data,
                      uint32_t version, uint32_t id)
{
    std::cout << "Shell Bind function Called" <<std::endl;
    struct wl_resource *resource = wl_resource_create(client, &motorcar_shell_interface, version, id);
    wl_resource_set_implementation(resource, &motorcarShellInterface, data, 0);
}
