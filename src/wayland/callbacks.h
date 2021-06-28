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

typedef std::function<void(struct wlr_xdg_surface * data)> NewSurfaceCallbackMethodXDG;
struct NewSurfaceCallbackXDG : GenericCallbackXDG {
	NewSurfaceCallbackXDG() {
		// printf("ctor: %p\n", this);
		listener = {{}, [](struct wl_listener *listener, void *data) {
			// printf("test %s\n", __PRETTY_FUNCTION__);
			GenericCallbackXDG *callbackStruct = wl_container_of(listener, callbackStruct, listener);
			// printf("ctor: %p\n", callbackStruct);
			intptr_t callbackPtr = (intptr_t) callbackStruct + sizeof(wl_listener);
			NewSurfaceCallbackMethodXDG *callback = (NewSurfaceCallbackMethodXDG *) callbackPtr;
			(*callback)((struct wlr_xdg_surface *) data);
		}};
	}
	NewSurfaceCallbackMethodXDG callback;
};

}