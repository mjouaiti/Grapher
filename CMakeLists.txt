# Specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8)

# Project's name

project(Grapher)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -std=c++11)

INCLUDE(FindOpenGL)
find_package(Threads REQUIRED)

set(GRAPHICAL_SOURCES src/Shader.cpp src/Grapher.cpp)

ADD_LIBRARY(Grapher STATIC ${GRAPHICAL_SOURCES})
add_definitions(-pthread)

IF(APPLE)
   INCLUDE_DIRECTORIES(/System/Library/Frameworks)
   INCLUDE_DIRECTORIES(/usr/local/lib)
   find_package(glfw3 3.2 REQUIRED)
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(IOKIT_LIBRARY IOKit)
   FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo)
   FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
   MARK_AS_ADVANCED (COCOA_LIBRARY
		     COREVIDEO_LIBRARY
		     IOKIT_LIBRARY
                     OpenGL_LIBRARY)
   SET(EXTRA_LIBS ${COCOA_LIBRARY} ${COREVIDEO_LIBRARY} ${IOKIT_LIBRARY} ${OpenGL_LIBRARY})
   target_link_libraries(Grapher ${EXTRA_LIBS} glfw3)
ELSE()
   find_package(OpenGL REQUIRED)
   find_package(GLEW REQUIRED)
   find_package(PkgConfig REQUIRED)
   pkg_search_module(GLFW REQUIRED glfw3)
   include_directories(${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
   target_link_libraries(Graphical ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} -ldl -lm ${CMAKE_THREAD_LIBS_INIT})
ENDIF (APPLE)

