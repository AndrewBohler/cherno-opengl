# cherno-opengl
I'm learning OpenGL by following The Cherno's YouTube series:

[![The Cherno: Welcome to OpenGL](https://img.youtube.com/vi/W3gAzLwfIP0/0.jpg)](https://youtu.be/W3gAzLwfIP0 "The Cherno: Welcome to OpenGL")

# Build using make:

`make dependencies` generate dependencies for source files in src/

`make vendor` force build source files in src/vendor

`make build` build source files in src/

`make run` runs the exe (needs to be run from project directory, not bin/!


This is currently set up to work with my system, using:

Windows 10 64-bit

mingw - C/C++ compiler (32-bit)
http://www.mingw.org/

GLEW - extension loader for OpenGL
http://glew.sourceforge.net/

GLFW3 - used to create an OpenGL context (window)
https://www.glfw.org/

A great reference for OpenGl functions:
http://docs.gl/
