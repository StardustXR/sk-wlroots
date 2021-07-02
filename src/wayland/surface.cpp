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

#define DENSITY 1000.0f

using namespace sk;

Surface::Surface(struct wlr_renderer *renderer, struct wlr_surface *surface) {
	this->renderer = renderer;
	this->surface = surface;

//	tex_t fakeTex = tex_create_file("/home/nova/Downloads/image.png");
	this->surfaceTex = tex_create();
	this->surfaceTex->type            = tex_type_image_nomips;
	this->surfaceTex->format          = tex_format_rgba32;

	this->surfaceTex->tex.type        = skg_tex_type_image;
	this->surfaceTex->tex.use         = skg_use_dynamic;
	this->surfaceTex->tex.mips        = skg_mip_none;
	this->surfaceTex->tex.format      = skg_tex_fmt_rgba32;
	this->surfaceTex->tex.array_count = 1;

	// this->surfaceShader = shader_create_file("/run/media/nova/MEDIA/xr/stardust/sk-wlroots/src/shaders/shader_unlit.sks");
	this->surfaceShader = shader_create_file("/run/media/nova/MEDIA/xr/stardust/sk-wlroots/src/shaders/shader_unlit_gamma.sks");

	this->surfaceMat = material_create(this->surfaceShader);
//	this->surfaceMat = material_copy_id(default_id_material_unlit);
//	material_set_texture(this->surfaceMat, "diffuse", fakeTex);
	material_set_transparency(this->surfaceMat, transparency_blend);
	material_set_texture(this->surfaceMat, "diffuse", this->surfaceTex);

	this->surfaceMesh = mesh_gen_plane(vec2_one, -vec3_forward, vec3_up);
}

Surface::~Surface() {}

void Surface::onMapped() {
	struct wlr_texture *surfaceTexture = wlr_surface_get_texture(surface);
	mesh_release(this->surfaceMesh);
	this->surfaceMesh = mesh_gen_plane(vec2{((float) surfaceTexture->width)/DENSITY, ((float) surfaceTexture->height)/DENSITY}, -vec3_forward, vec3_up);
}

GLuint tex = 0;

void readWaylandTexture(GLuint tex, GLenum target, GLenum format, uint32_t height, uint32_t width, void **data, size_t *size, GLuint *outTex) {
	*size = width * height * 4;
	*data = malloc(*size);
	memset(*data, uint8_t(127), *size);
	uint32_t fbo = 0;

	PFNGLFRAMEBUFFERTEXTUREOESPROC glFramebufferTextureOES = (PFNGLFRAMEBUFFERTEXTUREOESPROC) eglGetProcAddress("glFramebufferTextureOES");

	glGenFramebuffers          (1, &fbo);
	glBindFramebuffer          (GL_FRAMEBUFFER, fbo);
	(*glFramebufferTextureOES) (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);
	glFramebufferTexture2D     (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	glReadPixels(0, 0, width, height, format, GL_UNSIGNED_BYTE, *data);

	glBindFramebuffer   (GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fbo);

//	FILE *f = fopen("/tmp/image.raw", "wb");
//	fwrite(data, *size, 1, f);
//	fclose(f);
//	exit(1);

//	glDeleteTextures(1, outTex);
//	glGenTextures(1, outTex);
//	glBindTexture(GL_TEXTURE_2D, *outTex);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, *data);
//	glBlendColor(0, 1, 0.75, 1);
//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, *data);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	free(*data);
}

bool Surface::updateSurface() {
	struct wlr_texture *surfaceTexture = wlr_surface_get_texture(surface);
	if(!surfaceTexture || !wlr_texture_is_gles2(surfaceTexture)) {
		printf("Surface texture does not exist or is not GLES2\n");
		return false;
	}
	struct wlr_gles2_texture *eglTexture = (struct wlr_gles2_texture *) surfaceTexture;
	// if(eglTexture.target != GL_TEXTURE_2D)
		// return false;

//	size_t size;
//	void *data = nullptr;
//	readWaylandTexture(eglTexture->tex, eglTexture->target, GL_RGBA, surfaceTexture->height, surfaceTexture->width, &data, &size, &tex);

//	tex_set_colors(this->surfaceTex, surfaceTexture->width, surfaceTexture->height, data);
//	free(data);
//	tex_set_surface(this->surfaceTex, (void *) eglTexture->tex, sk::tex_type_image, GL_RGBA, surfaceTexture->width, surfaceTexture->height, 1);

	this->surfaceTex->tex.width       = surfaceTexture->width;
	this->surfaceTex->tex.height      = surfaceTexture->height;
//	glBindTexture(eglTexture->target, eglTexture->tex);
	this->surfaceTex->tex._texture    = eglTexture->tex;
//	this->surfaceTex->tex._texture    = tex;
	this->surfaceTex->tex._target     = eglTexture->target;
//	this->surfaceTex->tex._target     = GL_TEXTURE_EXTERNAL_OES;
	// printf("%b\n", eglTexture->target == GL_TEXTURE_EXTERNAL_OES);
	tex_set_options(this->surfaceTex, tex_sample_point, tex_address_clamp, 1);

	return true;
}

void Surface::draw() {
	if(!*mapped)
		return;
	if(updateSurface())
		render_add_mesh(this->surfaceMesh, this->surfaceMat, matrix_trs(vec3_forward * 0.5f));

	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	wlr_surface_send_frame_done(surface, &now);
}
