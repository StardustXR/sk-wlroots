#include "assert.h"
#include <algorithm>
#include <stereokit.h>

#include "callbacks.h"
#include "wayland.hpp"

extern "C" {

#define static
#include "wlr/types/wlr_compositor.h"
#include "wlr/render/gles2.h"
#include "wlr/render/egl.h"
#include "wlr/backend/noop.h"
#undef static

#include "wlr/types/wlr_data_device.h"
#include "wlr/types/wlr_output.h"
#include "wlr/types/wlr_output_layout.h"
#include "wlr/types/wlr_xdg_shell.h"
#include "xwaylandinclude.h"

#include "wlr/util/log.h"

}

static void wlr_log_handler(wlr_log_importance level, const char *fmt, va_list args) {
	switch (level) {
		case WLR_ERROR:
			sk::log_errf(fmt, args);
		break;

		case WLR_INFO:
			sk::log_infof(fmt, args);
		break;

		case WLR_DEBUG:
			sk::log_diagf(fmt, args);
		break;

		default:
	return;
	}
}

Wayland::Wayland(EGLDisplay display, EGLContext context, EGLenum platform) {
	wlr_log_init(WLR_DEBUG, wlr_log_handler);

	wayland_display = wl_display_create();
	assert(wayland_display);
	event_loop = wl_display_get_event_loop(wayland_display);
	assert(event_loop);

	const char *socket = wl_display_add_socket_auto(wayland_display);
	assert(socket);

	printf("Running compositor on wayland display '%s'\n", socket);
	setenv("WAYLAND_DISPLAY", socket, true);
	
	// wl_display_init_shm(display);
	egl = wlr_egl_from_context(display, context);
	assert(egl);

	renderer = wlr_gles2_renderer_create(egl);
	assert(renderer);
	wlr_renderer_init_wl_display(renderer, wayland_display);

	compositor = wlr_compositor_create(wayland_display, renderer);
	assert(compositor);

	backend = wlr_noop_backend_create(wayland_display);
	assert(backend);
	if (!wlr_backend_start(backend)) {
		fprintf(stderr, "Failed to start backend\n");
		wl_display_destroy(wayland_display);
		exit(1);
	}

	output = wlr_noop_add_output(backend);
	assert(output);
	output_layout = wlr_output_layout_create();
	wlr_output_layout_add(output_layout, output, 0, 0);
	wlr_output_set_scale(output, 2.0f);
	wlr_output_commit(output);

	// wlr_data_device_manager_create(display);
	xdg_shell = wlr_xdg_shell_create(wayland_display);
	assert(xdg_shell);
	newSurfaceCallbackXDG.callback = std::bind(&Wayland::onNewXDGSurface, this, std::placeholders::_1);
	wl_signal_add(&xdg_shell->events.new_surface, &newSurfaceCallbackXDG.listener);
	destroySurfaceCallbackXDG.callback = std::bind(&Wayland::onDestroyXDGSurface, this, std::placeholders::_1);

	xwayland = wlr_xwayland_create(wayland_display, compositor, false);
	assert(xwayland);
	newSurfaceCallbackXWayland.callback = std::bind(&Wayland::onNewXWaylandSurface, this, std::placeholders::_1);
	wl_signal_add(&xwayland->events.new_surface, &newSurfaceCallbackXWayland.listener);
	mapSurfaceCallbackXWayland.callback = std::bind(&Wayland::onMapXWaylandSurface, this, std::placeholders::_1);
	destroySurfaceCallbackXWayland.callback = std::bind(&Wayland::onDestroyXWaylandSurface, this, std::placeholders::_1);
}

Wayland::~Wayland() {}

void Wayland::update() {
	wl_display_flush_clients(wayland_display);
	wl_event_loop_dispatch(event_loop, 1);
}

void Wayland::destroyXWaylandSurface(wlr_surface *surface) {
	surfaces.erase(std::remove_if(
		surfaces.begin(),
		surfaces.end(),
		[surface](Surface *listSurface) {
			return listSurface->surface == surface;
		}),
		surfaces.end()
	);
}

void Wayland::onNewXDGSurface(void *data) {
	wlr_xdg_surface *surface = (wlr_xdg_surface *) data;
	wl_signal_add(&surface->events.destroy, &destroySurfaceCallbackXDG.listener);

	XDGSurface *newSurface = new XDGSurface(renderer, surface);

    xdgSurfaces.push_back(newSurface);
	surfaces.push_back(newSurface);
}

void Wayland::onDestroyXDGSurface(void *data) {
	wlr_xdg_surface *xdg_surface = (wlr_xdg_surface *) data;

	xdgSurfaces.erase(std::remove_if(
		xdgSurfaces.begin(),
		xdgSurfaces.end(),
		[xdg_surface](XDGSurface *surface) {
			return surface->xdg_surface == xdg_surface;
		}),
		xdgSurfaces.end()
	);
	destroyXWaylandSurface(xdg_surface->surface);
}

void Wayland::onNewXWaylandSurface(void *data) {
	wlr_xwayland_surface *surface = (wlr_xwayland_surface *) data;
	wl_signal_add(&surface->events.map, &mapSurfaceCallbackXWayland.listener);
	wl_signal_add(&surface->events.destroy, &destroySurfaceCallbackXWayland.listener);
}

void Wayland::onMapXWaylandSurface(void *data) {
	wlr_xwayland_surface *surface = (wlr_xwayland_surface *) data;
	XWaylandSurface *newSurface = new XWaylandSurface(renderer, surface);

	xWaylandSurfaces.push_back(newSurface);
	surfaces.push_back(newSurface);
}

void Wayland::onDestroyXWaylandSurface(void *data) {
	wlr_xwayland_surface *xwayland_surface = (wlr_xwayland_surface *) data;

	xWaylandSurfaces.erase(std::remove_if(
		xWaylandSurfaces.begin(),
		xWaylandSurfaces.end(),
		[xwayland_surface](XWaylandSurface *surface) {
			return surface->xwayland_surface == xwayland_surface;
		}),
		xWaylandSurfaces.end()
	);
	destroyXWaylandSurface(xwayland_surface->surface);
}