#pragma once
extern "C" {
#include "wlr/types/wlr_surface.h"
}
#include <stereokit.h>

class Surface {
public:
	explicit Surface(struct wlr_egl *egl, struct wlr_surface *surface);
	~Surface();

	void draw();

protected:
	struct wlr_egl *waylandEGL;
	struct wlr_surface *waylandSurface;

	bool updateSurface();

	sk::mesh_t surfaceMesh;
	sk::material_t surfaceMat;
	sk::tex_t surfaceTex;
};