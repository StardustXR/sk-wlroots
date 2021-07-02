#include "wayland.hpp"
#include "callbacks.h"
#include "assert.h"
#include <EGL/eglext.h>

extern "C" {
	#include "wlr/types/wlr_data_device.h"
}

Wayland::Wayland(EGLDisplay display, EGLContext context, EGLenum platform) {
	wl_display = wl_display_create();
	assert(wl_display);
	wl_event_loop = wl_display_get_event_loop(wl_display);
	assert(wl_event_loop);

	const char *socket = wl_display_add_socket_auto(wl_display);
	assert(socket);

	printf("Running compositor on wayland display '%s'\n", socket);
	setenv("WAYLAND_DISPLAY", socket, true);
	
	// wl_display_init_shm(wl_display);
	egl = wlr_egl_from_context(display, context);
	assert(egl);

	renderer = wlr_gles2_renderer_create(egl);
	assert(renderer);
	wlr_renderer_init_wl_display(renderer, wl_display);

	compositor = wlr_compositor_create(wl_display, renderer);
	assert(compositor);
	// wlr_data_device_manager_create(wl_display);
	xdg_shell = wlr_xdg_shell_create(wl_display);
	assert(xdg_shell);

	newSurfaceCallbackXDG.callback = std::bind(&Wayland::onNewXDGSurface, this, std::placeholders::_1);
	wl_signal_add(&xdg_shell->events.new_surface, &newSurfaceCallbackXDG.listener);
}

Wayland::~Wayland() {

}

void Wayland::update() {
	wl_display_flush_clients(wl_display);
	wl_event_loop_dispatch(wl_event_loop, 1);

	for(XDGSurface &surface : xdgSurfaces) {
		surface.draw();
	}
}

void Wayland::onNewXDGSurface(void *data) {
	struct wlr_xdg_surface *surface = (struct wlr_xdg_surface *) data;

    xdgSurfaces.emplace_back(renderer, surface);
	// if (surface->role != WLR_XDG_SURFACE_ROLE_TOPLEVEL)
		// return;
}
