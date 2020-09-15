#include "TestTexture3D.h"

#include "Renderer.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

namespace test {

const std::string TestTexture3D::m_Title("3D Texture Test");

const float TestTexture3D::m_Vertices[(3 + 3 + 2) * 8]{
    /*   3D location           rgb color      2D texture coords 
     *(x,     y,     z),   ( r,     g,     b),   (  u,     v)
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

const unsigned int TestTexture3D::m_Triangles[3 * 2 * 6]{
    0, 1, 2,   3, 2, 1, // front
    6, 5, 4,   5, 6, 7, // back
    6, 2, 3,   3, 7, 6, // left
    4, 5, 0,   1, 0, 5, // right
    0, 2, 4,   6, 4, 2, // top
    1, 5, 3,   7, 3, 5, // bottom
};

TestTexture3D::TestTexture3D(Renderer* renderer)
    : m_Renderer(renderer)
    , m_Shader("res/shaders/rotation_test.glsl")
    , m_Texture("res/textures/drew_blurry.png")
    , m_IB(nullptr), m_VB(nullptr)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_VBLayout.Push<float>(3); // 3D position
    m_VBLayout.Push<float>(3); // 3 channel color
    m_VBLayout.Push<float>(2); // 2D texture coords

    m_IB = new IndexBuffer(m_Triangles, 3 * 2 * 6);
    m_VB = new VertexBuffer(m_Vertices, 8 * m_VBLayout.GetStride());
    m_VA.AddBuffer(*m_VB, m_VBLayout);

    m_Shader.Bind();
    float temp_pos[]{2.0f, 0.0f, 0.0f, 0.0f};
    float temp_color[]{0.5f, 0.5f, 0.5f};
    m_Shader.SetUniform<float>("u_Pos", 4, temp_pos);
    m_Shader.SetUniform<float>("u_Color", 3, temp_color);
    m_Shader.SetUniformMat4f("u_MVP", 1, m_Renderer->GetMVP());
}

TestTexture3D::~TestTexture3D(){
    delete m_IB;
    delete m_VB;
}

void TestTexture3D::OnUpdate(float deltatime){
    m_Shader.Bind();
    m_Shader.SetUniformMat4f("u_MVP", 1, m_Renderer->GetMVP());
}

void TestTexture3D::OnRender(){
    m_Texture.Bind();
    m_Renderer->Draw(m_VA, *m_IB, m_Shader);
}

void TestTexture3D::OnImGUIRender(){
    ImGui::Text(m_Texture.GetFilePath().c_str());
    ImGui::Begin(m_Texture.GetFilePath().c_str());
    ImGui::Image((ImTextureID)m_Texture.GetID(),
        ImVec2((float)m_Texture.GetWidth(), (float)m_Texture.GetHeight()),
        ImVec2(1.0f, 1.0f), ImVec2(0.0f, 0.0f));
    ImGui::End();
}

} // namespace test
