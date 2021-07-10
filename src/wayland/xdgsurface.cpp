#include "xdgsurface.hpp"

extern "C" {
#include "types/wlr_xdg_shell.h"
}

XDGSurface::XDGSurface(wlr_renderer *renderer, wlr_xdg_surface *xdg_surface) : Surface(renderer, xdg_surface->surface) {
	this->xdg_surface = xdg_surface;

	if(wlr_surface_is_xdg_surface(surface))
		printf("New XDG surface created\n");

	mapped = &xdg_surface->mapped;

	wlr_xdg_toplevel_set_activated(xdg_surface, true);
	// wlr_xdg_toplevel_set_fullscreen(xdg_surface, true);
	wlr_xdg_toplevel_set_maximized(xdg_surface, true);
}

XDGSurface::~XDGSurface() {}
