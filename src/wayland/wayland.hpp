#pragma once

#include <vector>

#include <EGL/egl.h>
#include <wayland-server.h>

#include "callbacks.h"
#include "xdgsurface.hpp"

struct wlr_egl;
struct wlr_compositor;
struct wlr_backend;
struct wlr_output;
struct wlr_output_layout;
struct wlr_xdg_shell;

class Wayland {
public:
	explicit Wayland(EGLDisplay display, EGLContext context, EGLenum platform);
	~Wayland();

	void update();

	void onNewXDGSurface(void *data);
	std::vector<XDGSurface *> xdgSurfaces;

protected:
	wl_display *wayland_display;
	wl_event_loop *event_loop;

	wlr_egl *egl;
	wlr_renderer *renderer;
	wlr_compositor *compositor;
	wlr_backend *backend;
	wlr_output *output;
	wlr_output_layout *output_layout;

	wlr_xdg_shell *xdg_shell;
	WaylandCallback newSurfaceCallbackXDG;
};