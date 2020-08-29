cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Cherno-OpenGL VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# paths
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/lib)

# GLEW
message(STATUS "Adding subdirectory lib/glew")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
    add_definitions(-DGLEW_STATIC)
    add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glew EXCLUDE_FROM_ALL)
    target_link_libraries(main glew)
list(POP_BACK CMAKE_MESSAGE_INDENT)

# GLFW
message(STATUS "Adding subdirectory lib/glfw")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glfw EXCLUDE_FROM_ALL)
    target_link_libraries(main glfw)
list(POP_BACK CMAKE_MESSAGE_INDENT)

# GLM
message(STATUS "Adding subdirectory lib/glm")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
    add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glm EXCLUDE_FROM_ALL)
    target_link_libraries(main glm)
list(POP_BACK CMAKE_MESSAGE_INDENT)

# Include DIRs (g++ -I flags)
include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/vendor"
    "${CMAKE_SOURCE_DIR}/include"
)

# Add our sources
file(GLOB CPP_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

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

# Put all libraries into a variable
# (g++ -l flags)
set(LIBS
    gdi32
    glu32
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})
