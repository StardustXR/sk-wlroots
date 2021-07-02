#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "wayland/wayland.hpp"

#include "stereokit.h"
#include "sk_internal_defs.hpp"

Wayland *wayland;

using namespace sk;

#define DENSITY 1000.0f
mesh_t planeMesh;

int main() {
	sk_settings_t sk_settings = {};
	sk_settings.app_name = "sk-wlroots demo";
	sk_settings.flatscreen_width = 1600;
	sk_settings.flatscreen_height = 900;
	sk_settings.log_filter = sk::log_diagnostic;
	if (!sk_init(sk_settings))
		return 1;

	struct skg_platform_data_t stereokitPlatformData = skg_get_platform_data();

	planeMesh = mesh_gen_plane(vec2_one, -vec3_forward, vec3_up);
	wayland = new Wayland(stereokitPlatformData._egl_display, stereokitPlatformData._egl_context, EGL_PLATFORM_GBM_MESA);

	while (sk_step([]() {
		wayland->update();
		for(Surface &surface : wayland->xdgSurfaces) {
			if(*surface.mapped) {
				render_add_mesh(planeMesh, surface.surfaceMat, matrix_trs(vec3_forward * 0.5f, quat_identity, vec3{surface.width/DENSITY, surface.height/DENSITY, 1}));
			}
		}
	}));

	return 0;
}