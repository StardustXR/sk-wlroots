#pragma once

#include <wayland-server.h>
extern "C" {
#include "wlr/render/egl.h"
#define static
#include "wlr/render/gles2.h"
#undef static
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_xdg_shell.h>
}
#include <vector>

#include "callbacks.h"
#include "xdgsurface.hpp"

class Wayland {
public:
	explicit Wayland(EGLDisplay display, EGLContext context, EGLenum platform);
	~Wayland();

	void update();

	void onNewXDGSurface(void *data);

protected:
	wl_display *wayland_display;
	wl_event_loop *event_loop;

	wlr_egl *egl;
	wlr_renderer *renderer;
	wlr_compositor *compositor;

	wlr_xdg_shell *xdg_shell;
	WaylandCallback newSurfaceCallbackXDG;

	std::vector<XDGSurface> xdgSurfaces;
};