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
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/tests/*.cpp
)

# Add vendor sources
file(GLOB_RECURSE VENDOR_SOURCES
    ${CMAKE_SOURCE_DIR}/src/vendor/imgui/*.cpp
    ${CMAKE_SOURCE_DIR}/src/vendor/stb_image/*.cpp
)
	
# Add all header files
file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.hpp
)

# Output executable
add_executable(
    ${PROJECT_NAME}
    ${HEADER_FILES}
    ${VENDOR_SOURCES}
    ${CPP_SOURCES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# GLEW
message(STATUS "Adding subdirectory lib/glew")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
    add_definitions(-DGLEW_STATIC)
    add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
    target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)
list(POP_BACK CMAKE_MESSAGE_INDENT)

# GLFW
message(STATUS "Adding subdirectory lib/glfw")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
    # From the cmake instructions on glfw website:
    # When including GLFW as part of your build,
    # you probably don't want to build the GLFW tests,
    # examples and documentation. To disable these,
    # set the corresponding cache variables before adding the GLFW source tree.
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glfw EXCLUDE_FROM_ALL)
    target_include_directories(${PROJECT_NAME} PUBLIC glfw)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
list(POP_BACK CMAKE_MESSAGE_INDENT)

# GLM
message(STATUS "Adding subdirectory lib/glm")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
    add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glm EXCLUDE_FROM_ALL)
    target_link_libraries(${PROJECT_NAME} PRIVATE glm)
list(POP_BACK CMAKE_MESSAGE_INDENT)

# Include DIRs (g++ -I flags)
include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/vendor"
    "${CMAKE_SOURCE_DIR}/include"
)

# Put all libraries into a variable
# (g++ -l flags)
set(LIBS
    gdi32
    glu32
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})
