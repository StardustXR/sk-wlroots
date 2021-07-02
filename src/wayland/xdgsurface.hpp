#pragma once

#include "surface.hpp"
#include "types/wlr_xdg_shell.h"
#include "callbacks.h"

class XDGSurface : public Surface {
public:
    explicit XDGSurface(struct wlr_renderer *renderer, struct wlr_xdg_surface *surface);
	~XDGSurface();

protected:
	struct wlr_xdg_surface *xdg_surface;
};
