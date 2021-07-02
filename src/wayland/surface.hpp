#pragma once
extern "C" {
#include "wlr/types/wlr_surface.h"
}
#include "callbacks.h"
#include <stereokit.h>

class Surface {
public:
    explicit Surface(wlr_renderer *renderer, wlr_surface *surface);
	~Surface();

	void onCommit();

	bool *mapped = nullptr;
	sk::tex_t surfaceTex;
	sk::material_t surfaceMat;
	uint width;
	uint height;

protected:

	WaylandCallback surfaceMappedCallback;
	WaylandCallback surfaceCommitCallback;

    wlr_renderer *renderer;
	wlr_surface *surface;

	bool updateSurface();
	sk::shader_t surfaceShader;
};
