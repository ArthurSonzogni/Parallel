cmake_minimum_required(VERSION 2.6)
project(parallel)
set(EXECUTABLE_OUTPUT_PATH bin)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

# executable configuration
file(GLOB_RECURSE source_file src/*)
add_executable(parallel ${source_file})

# link configuration
target_link_libraries(
	parallel
	-l:libsfml-audio.so.2.1
	-l:libsfml-graphics.so.2.1
	-l:libsfml-window.so.2.1
	-l:libsfml-system.so.2.1
	)

if ( CMAKE_COMPILER_IS_GNUCC )
    # This is tested
    set_property( TARGET parallel  PROPERTY COMPILE_FLAGS -Wall )
endif ( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
    # This is untested
    set_property( TARGET parallel  PROPERTY COMPILE_FLAGS /W3 )
endif ( MSVC )
