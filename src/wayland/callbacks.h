#pragma once

#include <functional>
#include <stdint.h>
#include <wayland-server-core.h>

extern "C" {

#include <wlr/types/wlr_xdg_shell.h>


typedef std::function<void(void *)> WaylandCallbackMethod;
struct WaylandCallback {
	WaylandCallback() {
		listener = {{}, [](struct wl_listener *listener, void *data) {
			WaylandCallback *callbackStruct = wl_container_of(listener, callbackStruct, listener);
			intptr_t callbackPtr = (intptr_t) callbackStruct + sizeof(wl_listener);
			WaylandCallbackMethod *callback = (WaylandCallbackMethod *) callbackPtr;
			(*callback)(data);
		}};
	}

	struct wl_listener listener;
	WaylandCallbackMethod callback;
};

}