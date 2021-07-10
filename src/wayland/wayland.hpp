#pragma once

#include <vector>

#include <EGL/egl.h>
#include <wayland-server.h>

#include "callbacks.h"
#include "xdgsurface.hpp"
#include "xwaylandsurface.hpp"

struct wlr_egl;
struct wlr_compositor;
struct wlr_backend;
struct wlr_output;
struct wlr_output_layout;
struct wlr_xdg_shell;
struct wlr_xwayland;

class Wayland {
public:
	explicit Wayland(EGLDisplay display, EGLContext context, EGLenum platform);
	~Wayland();

	void update();

	void onNewXDGSurface(void *data);
	void onDestroyXDGSurface(void *data);
	std::vector<XDGSurface *> xdgSurfaces;

	void onNewXWaylandSurface(void *data);
	void onMapXWaylandSurface(void *data);
	void onDestroyXWaylandSurface(void *data);
	std::vector<XWaylandSurface *> xWaylandSurfaces;

	void destroyXWaylandSurface(wlr_surface *surface);
	std::vector<Surface *> surfaces;

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
	WaylandCallback destroySurfaceCallbackXDG;

	wlr_xwayland *xwayland;
	WaylandCallback newSurfaceCallbackXWayland;
	WaylandCallback mapSurfaceCallbackXWayland;
	WaylandCallback destroySurfaceCallbackXWayland;
};