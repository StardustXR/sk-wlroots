#include "surface.hpp"

#include <assert.h>
#include <cstddef>
#include <stdio.h>
#include <stereokit.h>

extern "C" {
#include "render/egl.h"
#define static
#include "render/gles2.h"
#undef static
}

namespace sk {
	extern void tex_set_surface(tex_t texture, void *native_surface, tex_type_ type, int64_t native_fmt, int32_t width, int32_t height, int32_t surface_count);
}

#define DENSITY 1000.0f

using namespace sk;

Surface::Surface(struct wlr_egl *egl, struct wlr_surface *surface) {
	this->waylandEGL = egl;
	this->waylandSurface = surface;

	this->surfaceTex = tex_create();

	this->surfaceMat = material_copy_id(default_id_material_unlit);
	material_set_texture(this->surfaceMat, "diffuse", this->surfaceTex);

	this->surfaceMesh = mesh_gen_plane(vec2_one, -vec3_forward, vec3_up);
}

Surface::~Surface() {}

bool Surface::updateSurface() {
	struct wlr_client_buffer *surfaceBuffer = waylandSurface->buffer;
	if(!surfaceBuffer)
		return false;
	struct wlr_texture *surfaceTexture = surfaceBuffer->texture;
	struct wlr_gles2_texture *surfaceTextureGLES = (wlr_gles2_texture *) surfaceTexture;
	assert(surfaceTextureGLES);

	tex_set_surface(this->surfaceTex, surfaceTextureGLES->image, sk::tex_type_image_nomips, surfaceTextureGLES->drm_format, surfaceTexture->width, surfaceTexture->height, 1);
	material_set_texture(this->surfaceMat, "diffuse", this->surfaceTex);
	// const size_t texSize = surfaceTexture->width*surfaceTexture->width*4;
	// void *texData = nullptr;
	// tex_get_data(this->surfaceTex, texData, texSize);

	mesh_release(this->surfaceMesh);
	this->surfaceMesh = mesh_gen_plane(vec2{((float) surfaceTexture->width)/DENSITY, ((float) surfaceTexture->height)/DENSITY}, -vec3_forward, vec3_up);

	return true;
}

void Surface::draw() {
	if(updateSurface())
		render_add_mesh(this->surfaceMesh, this->surfaceMat, matrix_trs(vec3_forward * 0.5f));
}