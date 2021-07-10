#pragma once
#include "callbacks.h"
#include <stereokit.h>

struct wlr_renderer;
struct wlr_surface;

class Surface {
public:
    explicit Surface(wlr_renderer *renderer, wlr_surface *surface);
	virtual ~Surface();

	void onCommit();

	virtual bool isMapped() const = 0;
	sk::tex_t surfaceTex;
	sk::material_t surfaceMat;
	uint width;
	uint height;
	wlr_surface *surface;

protected:

	WaylandCallback surfaceCommitCallback;

    wlr_renderer *renderer;

	bool updateSurface();
	sk::shader_t surfaceShader;
};
