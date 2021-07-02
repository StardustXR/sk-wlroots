#pragma once
extern "C" {
#include "wlr/types/wlr_surface.h"
}
#include "callbacks.h"
#include <stereokit.h>

class Surface {
public:
    explicit Surface(struct wlr_renderer *renderer, struct wlr_surface *surface);
	~Surface();

	protected: SurfaceMappedCallbackXDG surfaceMappedCallbackXDG;
	public: void onMapped();

	void draw();

protected:
    struct wlr_renderer *renderer;
	struct wlr_surface *surface;

	bool *mapped = nullptr;
	bool updateSurface();

	sk::mesh_t surfaceMesh;
	sk::material_t surfaceMat;
	sk::shader_t surfaceShader;
	sk::tex_t surfaceTex;
};
