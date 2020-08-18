#define GLEW_STATIC
#define GLFW_DLL // define this when static linking the gflw dll
// #define GLFW_INCLUDE_NONE // unsure if this is needed

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 1024
#endif
#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 720
#endif

#include <GL/glew.h>
// #include <GL/GLU.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <cstring>

#include "IndexBuffer.h"
#include "OpenGLDebugCallback.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

// comment 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

/* this is checked in main() */
#define ENABLE_GL_ERROR_CALLBACK true
#define DEBUG true

#define PI 3.14159265359

unsigned long long frame = 0;

// I forget why does this take in void? is this a typo?
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
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "cherno-opengl tutorial", NULL, NULL);
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
    }

    std::cout << "OpenGL version: ";
    std::cout << glGetString(GL_VERSION) << std::endl;

    std::cout << "\n*** Setting Up ***" << std::endl;
    std::cout << "------------------------------" << std::endl;

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1); // limit fps, doesn't seem to work...

    /* setup before rendering */
    const float vertices[(3 + 3 + 2) * 8]{
        /*   3D location           rgb color      2D texture coords 
         *(x,     y,     z),   ( r,     g,     b),   (  x,     y)
         */
         1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f, // 0
         1.0f, -1.0f,  1.0f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // 1
        -1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // 2
        -1.0f, -1.0f,  1.0f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f, // 3

         1.0f,  1.0f, -1.0f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f, // 4
         1.0f, -1.0f, -1.0f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f, // 5
        -1.0f,  1.0f, -1.0f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f, // 6
        -1.0f, -1.0f, -1.0f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f, // 7
    };

    const unsigned int triangles[3 * 2 * 6]{
        0, 1, 2,   3, 2, 1, // front
        6, 5, 4,   5, 6, 7, // back
        6, 2, 3,   3, 7, 6, // left
        4, 5, 0,   1, 0, 5, // right
        0, 2, 4,   6, 4, 2, // top
        1, 5, 3,   7, 3, 5, // bottom
    };

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexBufferLayout vb_layout;
    vb_layout.Push<float>(3); // 3D position
    vb_layout.Push<float>(3); // 3 channel color
    vb_layout.Push<float>(2); // 2D texture coords

    std::cout << "creating default vertex buffer" << std::endl;

    VertexArray va;
    VertexBuffer vb(vertices, 8 * vb_layout.GetStride());
    va.AddBuffer(vb, vb_layout);

    std::cout << "bulding vertex buffer layout" << std::endl;

    std::cout << "vb_layout:" << std::endl;
    std::cout << "\tstride: " << vb_layout.GetStride() << std::endl;
    int elcount = 0;
    for (VertexBufferElement element : vb_layout.GetElements()){
        std::cout << "\tElement " << elcount << std::endl;
        std::cout << "\t\ttype: " << element.type << std::endl;
        std::cout << "\t\tcount: " << element.count << std::endl;
        std::cout << "\t\tnormalized: " << (element.normalized ? "yes" : "no") << std::endl;
        elcount++;
    }

    std::cout << "creating default index buffer" << std::endl;
    IndexBuffer ib(triangles, 3 * 2 * 6);
    
    std::cout << "creating default shader" << std::endl;

    float min_width = -WINDOW_WIDTH / 2;
    float max_width = WINDOW_HEIGHT / 2;
    float min_height = -WINDOW_HEIGHT / 2;
    float max_height = WINDOW_HEIGHT / 2;
    float min_depth = min_height;
    float max_depth = max_height;

    glm::vec3 camera{0.0f, 0.0f, 0.0f};

    glm::mat4 proj = glm::ortho(min_width/100, max_width/100, min_height/100, max_height/100, min_depth/100, max_depth/100);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), camera);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
    glm::mat4 mvp = proj * view * model;

    Shader shader("res/shaders/rotation_test.glsl");
    shader.Bind();
    shader.SetUniformMat4f("u_MVP", 1, mvp);

    int u_Texture = 0;

    Texture texture("res/textures/drew_blurry.png");
    texture.Bind();
    shader.SetUniform<int>("u_Texture", 1, &u_Texture);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    std::cout << "creating renderer" << std::endl;
    Renderer renderer;

    std::cout << "setting up ImGui" << std::endl;
    /* set of Dear ImGui context */
    std::cout << "ImGui version " << IMGUI_CHECKVERSION() << std::endl;
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    /* Setup Platform/Renderer bindings */
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    /* Setup ImGui style */
    ImGui::StyleColorsDark();

    GLfloat ts = 0.0f;
    bool ts_manual = false;
    unsigned int id = 0;
    float color[3]{0.5f, 0.5f, 0.5f};
    float history[4][1000];

    std::cout << "\n*** begin render ***" << std::endl;
    std::cout << "------------------------------" << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Begin rendering here */
        renderer.Clear();
        for (int i = 0; i < 999; i++) {
            history[0][i] = history[0][i+1];
            history[1][i] = history[1][i+1];
            history[2][i] = history[2][i+1];
            history[3][i] = history[3][i+1];
        }
        history[0][999] = color[0];
        history[1][999] = color[1];
        history[2][999] = color[2];
        history[3][999] = (color[0] + color[1] + color[2]) / 3;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::Begin("Variables");
        ImGui::Text("Controls");
        if(ImGui::Button("Times Step Control")) ts_manual = !ts_manual;
        ImGui::SliderFloat("float", &ts, 0.0f, 100.0f);
        ImGui::SliderFloat("camera x", &camera[0], -10.0f, 10.0f);
        ImGui::SliderFloat("camera y", &camera[1], -10.0f, 10.0f);
        ImGui::SliderFloat("camera z", &camera[2], -10.0f, 10.0f);
        ImGui::ColorEdit3("multiply color", (float*)&color);
        ImGui::PlotHistogram("RGB", (float*)&color, 3, 0, "Color", 0.0f, 1.0f, ImVec2(300.0f, 100.0));
        ImGui::Text("time = %f", ts);
        ImGui::PlotLines("Red", history[0], 1000, 0, "", 0.0f, 1.0f, ImVec2(300.0f, 50.0f));
        ImGui::PlotLines("Green", history[1], 1000, 0, "", 0.0f, 1.0f, ImVec2(300.0f, 50.0f));
        ImGui::PlotLines("Blue", history[2], 1000, 0, "", 0.0f, 1.0f, ImVec2(300.0f, 50.0f));
        ImGui::PlotLines("Combined", history[3], 1000, 0, "", 0.0f, 1.0f, ImVec2(300.0f, 50.0f));

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        if(!ts_manual) ts = (float)glfwGetTime();
        
        color[0] = sin(ts                 ) / 2.0 + 0.5;
        color[1] = sin(ts + 2.0 * PI / 3.0) / 2.0 + 0.5;
        color[2] = sin(ts + 4.0 * PI / 3.0) / 2.0 + 0.5;

        glm::mat4 proj = glm::ortho(min_width/100, max_width/100, min_height/100, max_height/100, min_depth/100, max_depth/100);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), camera);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
        glm::mat4 mvp = proj * view * model;
        
        shader.Bind();
        texture.Bind();
        shader.SetUniform<float>("u_Time", 1, &ts);
        shader.SetUniform<float>("u_Color", 3, color);
        shader.SetUniform<int>("u_Texture", 1, &u_Texture);
        shader.SetUniformMat4f("u_MVP", 1, mvp);
        

        // for (unsigned int i = 0; i < 100; i++){
        //     shader.SetUniform<unsigned int>("u_ID", 1, &i);
        //     renderer.Draw(va, ib, shader);
        // }

        renderer.Draw(va, ib, shader);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        frame++;
    }
    std::cout << std::endl;

    std::cout << "\n*** cleaning up... ***" << std::endl;
    std::cout << "------------------------------" << std::endl;

    /* Cleanup ImGui */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    /* Close the OpenGL renderering context (GLFW) */
    glfwTerminate();
    return 0;
}