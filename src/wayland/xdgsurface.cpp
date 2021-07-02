#include "xdgsurface.hpp"
#include <wayland-server-core.h>

XDGSurface::XDGSurface(wlr_renderer *renderer, wlr_xdg_surface *xdg_surface) : Surface(renderer, xdg_surface->surface) {
	this->xdg_surface = xdg_surface;

	if(wlr_surface_is_xdg_surface(surface))
		printf("New XDG surface created\n");

	mapped = &xdg_surface->mapped;
	// surfaceMappedCallback.callback = std::bind(&Surface::onMapped, this);
	// wl_signal_add(&xdg_surface->events.map, &surfaceMappedCallback.listener);
}

XDGSurface::~XDGSurface() {}
