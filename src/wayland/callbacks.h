#pragma once

#include <functional>
#include <stdint.h>
#include <wayland-server-core.h>

extern "C" {

#include <wlr/types/wlr_xdg_shell.h>

}

// class Wayland;


extern "C" {

struct GenericCallbackXDG {
	struct wl_listener listener;
};


// new_surface xdg callback
typedef std::function<void(struct wlr_xdg_surface * data)> NewSurfaceCallbackMethodXDG;
struct NewSurfaceCallbackXDG : GenericCallbackXDG {
	NewSurfaceCallbackXDG() {
		listener = {{}, [](struct wl_listener *listener, void *data) {
			GenericCallbackXDG *callbackStruct = wl_container_of(listener, callbackStruct, listener);
			intptr_t callbackPtr = (intptr_t) callbackStruct + sizeof(wl_listener);
			NewSurfaceCallbackMethodXDG *callback = (NewSurfaceCallbackMethodXDG *) callbackPtr;
			(*callback)((struct wlr_xdg_surface *) data);
		}};
	}
	NewSurfaceCallbackMethodXDG callback;
};

typedef std::function<void(void)> SurfaceMappedCallbackMethodXDG;
struct SurfaceMappedCallbackXDG : GenericCallbackXDG {
	SurfaceMappedCallbackXDG() {
		listener = {{}, [](struct wl_listener *listener, void *data) {
			GenericCallbackXDG *callbackStruct = wl_container_of(listener, callbackStruct, listener);
			intptr_t callbackPtr = (intptr_t) callbackStruct + sizeof(wl_listener);
			SurfaceMappedCallbackMethodXDG *callback = (SurfaceMappedCallbackMethodXDG *) callbackPtr;
			(*callback)();
		}};
	}
	SurfaceMappedCallbackMethodXDG callback;
};

}