project('sk-wlroots','cpp', default_options : ['cpp_std=c++11'], version: '0.1.0')

# cmake = import('cmake')

# stereokit_project = cmake.subproject('StereoKit')
# stereokit_options = cmake.subproject_options()
# stereokit_options.set_install(false)
# stereokit = stereokit_project.dependency('StereoKitC', options: stereokit_options)
stereokit = dependency('StereoKitC', required: true)

src_includes = include_directories('src')
includes = [src_includes]

subdir('src')

executable(
	'sk-wlroots',
	'src/main.cpp',
	include_directories: includes,
	dependencies: [
		stereokit,
# After here are all the internal dependencies
		wayland,
	],
)
