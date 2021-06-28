#include "stereokit.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "wayland/wayland.hpp"

extern EGLDisplay egl_display;
extern EGLContext egl_context;

Wayland *wayland;

using namespace sk;

int main() {
	sk_settings_t sk_settings = {};
	sk_settings.app_name = "sk-wlroots demo";
	sk_settings.flatscreen_width = 1600;
	sk_settings.flatscreen_height = 900;
	if (!sk_init(sk_settings))
		return 1;

	wayland = new Wayland(egl_display, egl_context, EGL_PLATFORM_GBM_KHR);

	// wl_signal_add(&compositor.xdg_shell->events.new_surface, &compositor.xdg_shell_new_surface);

	while (sk_step([]() {
		wayland->update();
	}));

	return 0;
}