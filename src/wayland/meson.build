wayland_includes = include_directories('.')

lib_wayland = static_library(
	'wayland',
	files(
		# 'flatscreenpointer.cpp',
		# 'skhand.cpp',
	),
	include_directories: [wayland_includes],
	dependencies: [stereokit]
)

wayland = declare_dependency(
	include_directories: wayland_includes,
	link_with: lib_wayland
)
