#pragma once

#include <wayland-server.h>
extern "C" {
#include "wlr/render/egl.h"
#define static
#include "wlr/render/gles2.h"
#undef static
#include "wlr/backend/noop.h"
#include "wlr/types/wlr_compositor.h"
#include "wlr/types/wlr_output.h"
#include "wlr/types/wlr_output_layout.h"
#include "wlr/types/wlr_xdg_shell.h"
#include "wlr/util/log.h"
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
	std::vector<XDGSurface> xdgSurfaces;

protected:
	wl_display *wayland_display;
	wl_event_loop *event_loop;

	static void wlr_log_handler(wlr_log_importance level, const char *fmt, va_list args);

	wlr_egl *egl;
	wlr_renderer *renderer;
	wlr_compositor *compositor;
	wlr_backend *backend;
	wlr_output *output;
	wlr_output_layout *output_layout;

	wlr_xdg_shell *xdg_shell;
	WaylandCallback newSurfaceCallbackXDG;
};