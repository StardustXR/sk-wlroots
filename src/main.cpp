#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "wayland/wayland.hpp"

#include "stereokit.h"
#include "sk_internal_defs.hpp"

Wayland *wayland;

using namespace sk;

int main() {
	sk_settings_t sk_settings = {};
	sk_settings.app_name = "sk-wlroots demo";
	sk_settings.flatscreen_width = 1600;
	sk_settings.flatscreen_height = 900;
	sk_settings.log_filter = sk::log_diagnostic;
	if (!sk_init(sk_settings))
		return 1;

	struct skg_platform_data_t stereokitPlatformData = skg_get_platform_data();

	wayland = new Wayland(stereokitPlatformData._egl_display, stereokitPlatformData._egl_context, EGL_PLATFORM_GBM_MESA);

	while (sk_step([]() {
		wayland->update();
	}));

	return 0;
}