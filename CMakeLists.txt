cmake_minimum_required(VERSION 2.6)
project(parallel)
set(EXECUTABLE_OUTPUT_PATH bin)

# executable configuration
file(GLOB_RECURSE source_file src/*)
add_executable(parallel ${source_file})

# link configuration
target_link_libraries(
	parallel
	sfml-audio
	sfml-graphics
	sfml-window
	sfml-system
	)
