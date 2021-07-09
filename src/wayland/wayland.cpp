#include "assert.h"
#include <stereokit.h>

#include "callbacks.h"
#include "wayland.hpp"

extern "C" {

#include "wlr/render/egl.h"

#define static
#include "wlr/render/gles2.h"
#undef static

#include "wlr/backend/noop.h"
#include "wlr/types/wlr_compositor.h"
#include "wlr/types/wlr_data_device.h"
#include "wlr/types/wlr_output.h"
#include "wlr/types/wlr_output_layout.h"
#include "wlr/types/wlr_xdg_shell.h"
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
}

Wayland::~Wayland() {}

void Wayland::update() {
	wl_display_flush_clients(wayland_display);
	wl_event_loop_dispatch(event_loop, 1);
}

void Wayland::onNewXDGSurface(void *data) {
	wlr_xdg_surface *surface = (wlr_xdg_surface *) data;

	XDGSurface *newSurface = new XDGSurface(renderer, surface);

    xdgSurfaces.push_back(newSurface);
	surfaces.push_back(newSurface);
	if (surface->role != WLR_XDG_SURFACE_ROLE_TOPLEVEL)
		return;
}