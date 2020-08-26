cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Cherno-OpenGL VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# paths
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/lib)

# Add our sources
file(GLOB CPP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add vendor sources
file(GLOB_RECURSE VENDOR_SOURCES
    ${CMAKE_SOURCE_DIR}/src/vendor/imgui/*.cpp
    ${CMAKE_SOURCE_DIR}/src/vendor/stb_image/*.cpp)
	
# Add all header files
file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.hpp)

link_directories(${CMAKE_SOURCE_DIR}/dep/lib/win32)

add_library(glfw3 STATIC dep/lib/win32/glfw3.dll)
add_library(glfw3ddl STATIC dep/lib/win32/libglfw3.a)
add_library(gdi32 SHARED)
add_library(glu32 SHARED)
add_library(glew32 SHARED C:\OpenGL\bin\Release\Win32)
add_library(opengl32 SHARED C:/OpenGL/)

add_executable(${PROJECT_NAME} ${HEADER_FILES}
    ${VENDOR_SOURCES} ${CPP_SOURCES})

    # We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#######################################
# LOOK for the packages that we need! #
#######################################

# OpenGL
find_package(OpenGL REQUIRED)

# GDI debugger lib
find_package(GDI)
message(STATUS "GDI included at ${GDI_INCLUDE_DIR}")

# GLM math lib works nice with opengl and glsl
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# GLEW opengl function loader lib
find_package(GLEW REQUIRED)
message(STATUS "GLEW included at ${GLEW_INCLUDE_DIR}")

# GLFW opengl window creation lib
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# STB_IMAGE
add_library(STB_IMAGE "src/vendor/stb_image/stb_image.cpp")
message(STATUS "STB_IMGAGE inlcuded at ${STB_IMAGE_INCLUDE_DIR}")

# Put all libraries into a variable
set(LIBS glfw3 gdi32 glew32 opengl32 STB_IMAGE)

# Define the include DIRs
include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/vendor"
    "${CMAKE_SOURCE_DIR}/include"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})

