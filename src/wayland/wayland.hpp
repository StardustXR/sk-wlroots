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
#include "surface.hpp"

class Wayland {
public:
	explicit Wayland(EGLDisplay display, EGLContext context, EGLenum platform);
	~Wayland();

	void update();

	void newSurface(struct wlr_xdg_surface *surface);

protected:
	struct wl_display *wl_display;
	struct wl_event_loop *wl_event_loop;

	struct wlr_egl *egl;
	struct wlr_renderer *renderer;
	struct wlr_compositor *compositor;

	struct wlr_xdg_shell *xdg_shell;
	NewSurfaceCallbackXDG newSurfaceCallbackXDG;

	std::vector<Surface> surfaces;
};