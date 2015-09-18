cmake_minimum_required (VERSION 2.8.11)
project (Motorcar)
set (Motorcar_VERSION_MAJOR 0)
set (Motorcar_VERSION_MINOR 1)

include_directories(${CMAKE_SOURCE_DIR}/src/compositor)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#########################################################
# Motorcar protocol extensions
#########################################################

find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND_CLIENT wayland-client REQUIRED)
pkg_check_modules(WAYLAND_SERVER wayland-server REQUIRED)

find_program(WAYLAND_SCANNER NAMES wayland-scanner)

add_custom_command(
    OUTPUT  motorcar-client-protocol.h
    COMMAND ${WAYLAND_SCANNER} client-header
            < ${CMAKE_SOURCE_DIR}/src/protocol/motorcar.xml
            > ${CMAKE_CURRENT_BINARY_DIR}/motorcar-client-protocol.h
    DEPENDS ${CMAKE_SOURCE_DIR}/src/protocol/motorcar.xml
)

add_custom_command(
    OUTPUT  motorcar-server-protocol.h
    COMMAND ${WAYLAND_SCANNER} server-header
            < ${CMAKE_SOURCE_DIR}/src/protocol/motorcar.xml
            > ${CMAKE_CURRENT_BINARY_DIR}/motorcar-server-protocol.h
    DEPENDS ${CMAKE_SOURCE_DIR}/src/protocol/motorcar.xml
)

add_custom_command(
    OUTPUT  motorcar-wayland-extensions.c
    COMMAND ${WAYLAND_SCANNER} code
            < ${CMAKE_SOURCE_DIR}/src/protocol/motorcar.xml
            > ${CMAKE_CURRENT_BINARY_DIR}/motorcar-wayland-extensions.c
    DEPENDS ${CMAKE_SOURCE_DIR}/src/protocol/motorcar.xml
)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
SET(LIBRARIES ${LIBRARIES} ${OPENGL_LIBRARIES})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})
if (NOT GLEW_FOUND)
    message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)


#########################################################
# libmotorcar-server
#########################################################

add_library(motorcar-compositor SHARED

		motorcar-server-protocol.h
		motorcar-wayland-extensions.c

 		src/compositor/shell.h 
		src/compositor/displayserver.h 
		src/compositor/windowmanager.h 		
		src/compositor/motorcar.h
		src/compositor/geometry.h 
		src/compositor/compositor.h

		src/compositor/scenegraph/scenegraphnode.h 
		src/compositor/scenegraph/scenegraph.h 
		src/compositor/scenegraph/physicalnode.h 
		src/compositor/scenegraph/virtualnode.h 
		src/compositor/scenegraph/scene.h 

		src/compositor/scenegraph/output/drawable.h 
		src/compositor/scenegraph/output/wayland/waylandsurfacenode.h 
		src/compositor/scenegraph/output/output.h 
		src/compositor/scenegraph/output/display/display.h 
		src/compositor/scenegraph/output/display/rendertotexturedisplay.h 
		src/compositor/scenegraph/output/wireframenode.h 
		src/compositor/scenegraph/output/viewpoint.h
		src/compositor/scenegraph/output/wayland/motorcarsurfacenode.h 

		src/compositor/scenegraph/input/input.h
		src/compositor/scenegraph/input/bone.h 
		src/compositor/scenegraph/input/bonesensor.h 
		src/compositor/scenegraph/input/skeleton.h 
		src/compositor/scenegraph/input/singlebonetracker.h 
		src/compositor/scenegraph/input/sixdofpointingdevice.h 

		src/compositor/gl/openglshader.h 
		src/compositor/gl/GLSLHelper.h 
		src/compositor/gl/openglcontext.h 
		src/compositor/gl/viewport.h 

		src/compositor/events/event.h 
		src/compositor/events/mouseevent.h 
		src/compositor/events/events.h 
		src/compositor/events/keyboardevent.h 
		src/compositor/events/sixdofevent.h 

		src/compositor/wayland/input/seat.h 
		src/compositor/wayland/input/pointer.h 
		src/compositor/wayland/input/keyboard.h 
		src/compositor/wayland/input/waylandinput.h 
		src/compositor/wayland/output/waylandsurface.h 



		src/compositor/geometry.cpp 		
		src/compositor/compositor.cpp
		src/compositor/displayserver.cpp 
		src/compositor/windowmanager.cpp 
		src/compositor/shell.cpp  

		src/compositor/scenegraph/scenegraphnode.cpp 
		src/compositor/scenegraph/physicalnode.cpp 
		src/compositor/scenegraph/virtualnode.cpp 
		src/compositor/scenegraph/scene.cpp 

		src/compositor/scenegraph/output/drawable.cpp 
		src/compositor/scenegraph/output/wayland/waylandsurfacenode.cpp 	
		src/compositor/scenegraph/output/display/display.cpp 
		src/compositor/scenegraph/output/display/rendertotexturedisplay.cpp 
		src/compositor/scenegraph/output/wireframenode.cpp 
		src/compositor/scenegraph/output/viewpoint.cpp 
		src/compositor/scenegraph/output/wayland/motorcarsurfacenode.cpp

		src/compositor/scenegraph/input/bone.cpp 
		src/compositor/scenegraph/input/bonesensor.cpp 
		src/compositor/scenegraph/input/skeleton.cpp 
		src/compositor/scenegraph/input/singlebonetracker.cpp 
		src/compositor/scenegraph/input/sixdofpointingdevice.cpp 

		src/compositor/gl/openglshader.cpp 
		src/compositor/gl/GLSLHelper.cpp 
		src/compositor/gl/openglcontext.cpp 
		src/compositor/gl/viewport.cpp 

		src/compositor/events/event.cpp 
		src/compositor/events/mouseevent.cpp 
		src/compositor/events/keyboardevent.cpp 
		src/compositor/events/sixdofevent.cpp 

		src/compositor/wayland/input/seat.cpp 
		src/compositor/wayland/input/pointer.cpp 
		src/compositor/wayland/input/keyboard.cpp 

		src/compositor/wayland/output/waylandsurface.cpp 
)

set_property(TARGET motorcar-compositor PROPERTY CXX_STANDARD 11)
set_property(TARGET motorcar-compositor PROPERTY CXX_STANDARD_REQUIRED ON)

