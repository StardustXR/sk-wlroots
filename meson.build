project('sk-wlroots','cpp', default_options : ['c_std=c17', 'cpp_std=c++11'], version: '0.1.0')

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

lib_files = [server_protocols]
subdir('src')


lib_sk_wlroots = library(
	'sk-wlroots',
	lib_files,
	include_directories: includes,
	dependencies: [
		stereokit,
# After here are all the internal dependencies
		wlroots,
	],
	version: '0.1.0',
	soversion: '0',
)

sk_wlroots = declare_dependency(
	include_directories: includes,
	link_with: lib_sk_wlroots,
	dependencies: [
		wlroots,
	],
)

executable(
	'sk-wlroots',
	'src/main.cpp',
	include_directories: includes,
	dependencies: [
		stereokit,
		sk_wlroots
	],
)
