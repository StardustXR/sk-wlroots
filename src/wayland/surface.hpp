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

	void onMapped();
	void onCommit();
	void draw();

	bool *mapped = nullptr;
	sk::tex_t surfaceTex;

protected:

	WaylandCallback surfaceMappedCallback;
	WaylandCallback surfaceCommitCallback;

    struct wlr_renderer *renderer;
	struct wlr_surface *surface;

	bool updateSurface();

	sk::mesh_t surfaceMesh;
	sk::material_t surfaceMat;
	sk::shader_t surfaceShader;
};
