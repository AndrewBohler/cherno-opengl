// this is likely defined by the compiler
// #define GLEW_STATIC

// I think cmake handles this...
// #define GLFW_DLL // define this when static linking the gflw dll
// #define GLFW_INCLUDE_NONE // unsure if this is needed

/* vvv ordered includes vvv */
#include <GL/glew.h>
// #include <GL/GLU.h>
#include <GLFW/glfw3.h>
/* ^^^ ordered includes ^^^ */

#include <exception>
#include <cstring>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>

#include "IndexBuffer.h"
#include "OpenGLDebugCallback.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#ifdef BUILD_TESTS
#include "tests/Test.h"
#include "tests/TestAnimatePlane.h"
#include "tests/ClearColorTest.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTexture3D.h"
#include "tests/TestWaterTexture.h"
#endif // BUILD_TESTS

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

/* main.cpp defines */
#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 1024
#endif
#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 720
#endif

#define ENABLE_GL_ERROR_CALLBACK true
#define DEBUG true

#define PI 3.14159265359

unsigned long long frame = 0;

int main(void){ // I forget why does this take in void? is this a typo?
    
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

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float min_width = -WINDOW_WIDTH / 2;
    float max_width = WINDOW_HEIGHT / 2;
    float min_height = -WINDOW_HEIGHT / 2;
    float max_height = WINDOW_HEIGHT / 2;
    float min_depth = min_height;
    float max_depth = max_height;

    /* Model View Projection matrix with glm */
    // glm::vec3 camera{0.0f, 0.0f, 0.0f};
    // glm::mat4 proj = glm::ortho(min_width/100, max_width/100, min_height/100, max_height/100, min_depth/100, max_depth/100);
    // glm::mat4 view = glm::translate(glm::mat4(1.0f), camera);
    // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
    // glm::mat4 mvp = proj * view * model;

    std::cout << "creating renderer" << std::endl;
    Renderer renderer;

    std::cout << "setting up ImGui" << std::endl;
    /* setup Dear ImGui context */
    std::cout << "ImGui version " << IMGUI_CHECKVERSION() << std::endl;
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    /* Setup Platform/Renderer bindings */
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    /* Setup Dear ImGui style */
    ImGui::StyleColorsDark();

    #ifdef BUILD_TESTS
    std::stringstream debug_stream;
    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest, &renderer);
    currentTest = testMenu;
    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
    testMenu->RegisterTest<test::TestAnimatePlane>("2D Plane Animation");
    testMenu->RegisterTest<test::TestWaterTexture>("Water Texture Animation");
    testMenu->RegisterTest<test::TestTexture3D>("3D Texture");

    #endif // BUILD_TESTS

    std::cout << "\n*** begin render ***" << std::endl;
    std::cout << "------------------------------" << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Begin rendering here */
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        #ifdef BUILD_TESTS
            if (currentTest) {
                ImGui::Begin(currentTest->GetTitle().c_str());
                try {
                    currentTest->OnUpdate((float)glfwGetTime());
                    currentTest->OnRender();
                    if (currentTest != testMenu && ImGui::Button("<-")){
                        delete currentTest;
                        currentTest = testMenu;
                    }
                    currentTest->OnImGUIRender();
                } catch (std::exception e) {
                    std::cout << "[error] exception occured durting: " << currentTest->GetTitle() << std::endl;
                    std::cout << "-- " << e.what() << std::endl;
                    debug_stream << "[error] exception occured durting: " << currentTest->GetTitle() << "\n";
                    debug_stream << "-- " << e.what() << "\n";
                    delete currentTest;
                    currentTest = testMenu;
                } catch (...) {
                    std::cout << "[error] unkown exception!" << std::endl;
                    debug_stream << "[error] unkown exception!" << "\n";
                    delete currentTest;
                    currentTest = testMenu;
                }
                ImGui::End();
            }

        ImGui::Begin("Debug");
        ImGui::TextWrapped(debug_stream.str().c_str());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        #endif // BUILD_TESTS

        // glm::mat4 proj = glm::perspective(45.0f, ((float)WINDOW_WIDTH/(float)WINDOW_HEIGHT), 1.0f, 100.0f);
        // glm::mat4 view = glm::translate(glm::mat4(1.0f), camera);
        // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
        // glm::mat4 mvp = proj * view * model;

        ImGui::ShowMetricsWindow();

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

    /* Cleanup Dear ImGui */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    /* Close the OpenGL renderering context (GLFW) */
    glfwTerminate();
    return 0;
}