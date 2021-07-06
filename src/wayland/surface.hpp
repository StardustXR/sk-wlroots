#pragma once
#include "callbacks.h"
#include <stereokit.h>

struct wlr_renderer;
struct wlr_surface;

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

	WaylandCallback surfaceCommitCallback;

    wlr_renderer *renderer;
	wlr_surface *surface;

	bool updateSurface();
	sk::shader_t surfaceShader;
};
