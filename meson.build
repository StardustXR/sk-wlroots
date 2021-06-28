project('sk-wlroots','cpp', default_options : ['c_std=c17', 'cpp_std=c++11'], version: '0.1.0')

# cmake = import('cmake')

# stereokit_project = cmake.subproject('StereoKit')
# stereokit_options = cmake.subproject_options()
# stereokit_options.set_install(false)
# stereokit = stereokit_project.dependency('StereoKitC', options: stereokit_options)
stereokit = dependency('StereoKitC', required: true)

add_project_arguments([
	'-DWLR_USE_UNSTABLE',
	'-Dwlroots:gles2-renderer=enabled',
], language: 'cpp')

wlroots_proj = subproject('wlroots')
wlroots = wlroots_proj.get_variable('wlroots')
wlroots_server_protocols = wlroots_proj.get_variable('protocols_server_header')

server_protocols = [
	wlroots_server_protocols['xdg-shell']
]

src_includes = include_directories('src')
includes = [src_includes]

source_files = files('src/main.cpp')

subdir('src')

source_files += server_protocols

executable(
	'sk-wlroots',
	source_files,
	include_directories: includes,
	dependencies: [
		stereokit,
# After here are all the internal dependencies
		# wayland,
		wlroots,
	],
)
