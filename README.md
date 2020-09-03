# cherno-opengl
I'm learning OpenGL by following The Cherno's YouTube series:

[![The Cherno: Welcome to OpenGL](https://img.youtube.com/vi/W3gAzLwfIP0/0.jpg)](https://youtu.be/W3gAzLwfIP0 "The Cherno: Welcome to OpenGL")

# Build using CMake:

Note: Only tested on Windows 10 using MinGW and make!

Clone repo, make build directory and enter it

Don't forget the `--recurse-submodules` flag! If you do try `git submodule update --init --recursive`, submodules are located in `cherno-opengl/lib`.

```
git clone --recurse-submodules https://github.com/AndrewBohler/cherno-opengl.git
cd cherno-opengl
mkdir build
cd build
```

If you are on windows and want CMake to choose your compiler for you (it will probably default to a version of Visual Studio)

```cmake ..```

If you want to use a different compiler, e.g. MinGW with make

```
cmake -G "MinGW Makefiles" ..
make
```

The executable will be located in `cherno-opengl/bin`

--------

This is currently set up to work with my system, using:

Windows 10 64-bit

CMake - Tool for cross-platform builds
https://cmake.org/

GNU Make - Compilation Tool (32-bit)
https://www.gnu.org/software/make/

MinGW - C/C++ compiler (32-bit)
http://www.mingw.org/

GLEW - extension loader for OpenGL
http://glew.sourceforge.net/
(submodule in `lig/glew`)

GLFW3 - used to create an OpenGL context (window)
https://www.glfw.org/
(submodule in `lib/glfw`)

A great reference for OpenGl functions:
http://docs.gl/
