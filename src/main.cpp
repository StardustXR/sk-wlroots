#include "stereokit.h"
#include <wayland-server.h>

extern "C" {
#include "wlr/render/egl.h"
#define static
#include "wlr/render/gles2.h"
#undef static
#include <wlr/types/wlr_compositor.h>
}

using namespace sk;

typedef struct {
	struct wl_display *wl_display;
	struct wl_event_loop *wl_event_loop;

	struct wlr_renderer *renderer;
	struct wlr_compositor *compositor;
} wlroots_compositor;

wlroots_compositor compositor;

extern EGLDisplay egl_display;
extern EGLContext egl_context;

int main() {
	sk_settings_t sk_settings = {};
	sk_settings.app_name = "sk-wlroots demo";
	sk_settings.flatscreen_width = 1600;
	sk_settings.flatscreen_height = 900;
	if (!sk_init(sk_settings))
		return 1;

	compositor.wl_display = wl_display_create();
	compositor.wl_event_loop = wl_display_get_event_loop(compositor.wl_display);

	struct wlr_egl *egl = wlr_egl_from_context(egl_display, egl_context, EGL_PLATFORM_GBM_KHR);
	compositor.renderer = wlr_gles2_renderer_create(egl);

	compositor.compositor = wlr_compositor_create(compositor.wl_display, compositor.renderer);

	while (sk_step([]() {
	}));

	return 0;
}