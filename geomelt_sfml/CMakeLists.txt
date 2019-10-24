cmake_minimum_required(VERSION 2.6)
project(myproject)

# Set version information in a config.h file
set (CMAKE_CXX_STANDARD 17)
#set(myproject_VERSION_MAJOR 1)
#set(myproject_VERSION_MINOR 0)
include_directories("${PROJECT_BINARY_DIR}")

#Custom
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lsfml-graphics -lsfml-window -lsfml-system -Wall -lX11 -lGL -lGLU -lm")

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
#target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
  target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
endif()

set( HEADERS
	assets.h
	camera.h
	command.h
	complex_shapes.h
	headers.h
	input.h
	levels.h
	menus.h
	player.h
	primitives.h
	scenery.h
	state.h
	sync.h
)

set( SRC
	assets.cpp
	camera.cpp
	command.cpp
	complex_shapes.cpp
	input.cpp
	levels.cpp
	menus.cpp
	player.cpp
	primitives.cpp
	scenery.cpp
	state.cpp
	sync.cpp
)


# Define sources and executable
set(EXECUTABLE_NAME "myproject")
add_executable(${EXECUTABLE_NAME} ${HEADERS} ${SRC} )

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML REQUIRED system window graphics network audio)
target_link_libraries( ${EXECUTABLE_NAME} ${SFML_LIBRARIES})

