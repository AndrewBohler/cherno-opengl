// #define GLEW_STATIC
#define GLFW_DLL // define this when static linking the gflw dll
// #define GLFW_INCLUDE_NONE // unsure if this is needed

#include <glew.h>
#include <GL/GLU.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

#include "IndexBuffer.h"
#include "OpenGLDebugCallback.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

// #include "shapes.h"

/* this is checked in main() */
#define ENABLE_GL_ERROR_CALLBACK true
#define DEBUG true

#define ABS_PATH_SRC "C:/Users/drewb/OneDrive/Documents/CPP/cherno-opengl/src/"
#define REL_PATH_SHADER "res/shaders/rotation_test.glsl"

#define PI 3.14159265359

unsigned long long frame = 0;

int main(void){
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()){
        std::cout << "glfw failed to initialize!" << std::endl;
        system("pause");
        return -1;
    }

    /* enable debug context */
    if (ENABLE_GL_ERROR_CALLBACK) {
        std::cout << "hinting opengl debug context to glfw" << std::endl;
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 720, "cherno-opengl tutorial", NULL, NULL);
    if (!window){
        glfwTerminate();
        std::cout << "window creation failed!" << std::endl;
        system("pause");
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* glewInit() needs to be called after glfwMakeContextCurrent(), but not before! */
    GLenum glew_init_status = glewInit();
    std::cout << "glew init: " << glewGetErrorString(glew_init_status) << std::endl;

    /* check debug context */
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    std::cout << "GLFW_OPENGL_DEBUG_CONTEXT " << (flags & GL_CONTEXT_FLAG_DEBUG_BIT ? "true" : "false") << std::endl;


    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT){
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        std::cout << "GL_DEBUG_OUTPUT enabled" << std::flush;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1); // limit fps, doesn't seem to work...

    std::cout << "OpenGL version ";
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << "begin render\n--------------" << std::endl;

    /* setup before rendering */
    const float vertices[6 * 8]{
        /* (x,     y,     z)     (r,     g,     b) */
         1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f, // 0
         1.0f, -1.0f,  1.0f,   1.0f,  0.0f,  0.0f, // 1
        -1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f, // 2
        -1.0f, -1.0f,  1.0f,   1.0f,  0.0f,  0.0f, // 3

         1.0f,  1.0f, -1.0f,   0.0f,  0.0f,  1.0f, // 4
         1.0f, -1.0f, -1.0f,   0.0f,  0.0f,  1.0f, // 5
        -1.0f,  1.0f, -1.0f,   0.0f,  0.0f,  1.0f, // 6
        -1.0f, -1.0f, -1.0f,   0.0f,  0.0f,  1.0f, // 7
    };

    const unsigned int triangles[3 * 2 * 6]{
        0, 1, 2,   3, 2, 1, // front
        6, 5, 4,   5, 6, 7, // back
        6, 2, 3,   3, 7, 6, // left
        4, 5, 0,   1, 0, 5, // right
        0, 2, 4,   6, 4, 2, // top
        1, 5, 3,   7, 3, 5, // bottom
    };


    VertexArray va;
    VertexBuffer vb(vertices, 6 * 8 * sizeof(float));

    VertexBufferLayout vb_layout;
    vb_layout.Push<float>(3);
    vb_layout.Push<float>(3);
    va.AddBuffer(vb, vb_layout);

    IndexBuffer ib(triangles, 3 * 2 * 6);
    
    Shader shader(ABS_PATH_SRC REL_PATH_SHADER);
    shader.Bind();

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    // float r, g, b;
    GLfloat ts = 0.0f;
    unsigned int id;
    float color[3];
    std::string u_Name;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Begin rendering here */
        renderer.Clear();
        
        color[0] = sin(ts) / 2.0 + 0.5;
        color[1] = sin(ts + 2.0 * PI / 3.0) / 2.0 + 0.5;
        color[2] = sin(ts + 4.0 * PI / 3.0) / 2.0 + 0.5;

        ts = (float)glfwGetTime();
        id = (unsigned int)ts;

        shader.Bind();
        shader.SetUniform<float>("u_Time", 1, &ts);
        shader.SetUniform<unsigned int>("u_ID", 1, &id);
        shader.SetUniform<float>("u_Color", 3, color);

        // glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, nullptr);
        renderer.Draw(va, ib, shader);
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        frame++;
    }
    std::cout << std::endl;

    glfwTerminate();
    return 0;
}