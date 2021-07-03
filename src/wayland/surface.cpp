#include "surface.hpp"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <assert.h>
#include <cstddef>
#include <cstring>
#include <stdio.h>
#include <stereokit.h>

extern "C" {
#include "render/egl.h"
#define static
#include "render/gles2.h"
#undef static
#include "types/wlr_xdg_shell.h"
}

#include "sk_internal_defs.hpp"

using namespace sk;

Surface::Surface(wlr_renderer *renderer, wlr_surface *surface) {
	this->renderer = renderer;
	this->surface = surface;

	this->surfaceTex = tex_create();
	this->surfaceTex->type            = tex_type_image_nomips;
	this->surfaceTex->format          = tex_format_rgba32;

	this->surfaceTex->tex.type        = skg_tex_type_image;
	this->surfaceTex->tex.use         = skg_use_dynamic;
	this->surfaceTex->tex.mips        = skg_mip_none;
	this->surfaceTex->tex.format      = skg_tex_fmt_rgba32;
	this->surfaceTex->tex.array_count = 1;

	// this->surfaceShader = shader_create_file("/run/media/nova/MEDIA/xr/stardust/sk-wlroots/src/shaders/shader_unlit.sks");
	// this->surfaceShader = shader_create_file("/run/media/nova/MEDIA/xr/stardust/sk-wlroots/src/shaders/shader_unlit_gamma.sks");
	this->surfaceShader = shader_create_file("/run/media/nova/MEDIA/xr/stardust/sk-wlroots/src/shaders/shader_unlit_simula.sks");

	this->surfaceMat = material_create(this->surfaceShader);
	material_set_transparency(this->surfaceMat, transparency_blend);
	material_set_texture(this->surfaceMat, "diffuse", this->surfaceTex);

	surfaceCommitCallback.callback = std::bind(&Surface::onCommit, this);
	wl_signal_add(&surface->events.commit, &surfaceCommitCallback.listener);
}

Surface::~Surface() {}

void Surface::onCommit() {
	wlr_texture *surfaceTexture = wlr_surface_get_texture(surface);
	if(!surfaceTexture || !wlr_texture_is_gles2(surfaceTexture))
		return;
	wlr_gles2_texture *eglTexture = (wlr_gles2_texture *) surfaceTexture;

	this->width                       = surfaceTexture->width;
	this->height                      = surfaceTexture->height;

	this->surfaceTex->tex.width       = surfaceTexture->width;
	this->surfaceTex->tex.height      = surfaceTexture->height;
	this->surfaceTex->tex._texture    = eglTexture->tex;
	this->surfaceTex->tex._target     = eglTexture->target;
	tex_set_options(this->surfaceTex, tex_sample_linear, tex_address_clamp, 1);

	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	wlr_surface_send_frame_done(surface, &now);
}
