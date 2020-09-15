#include "TestTexture2D.h"

#include <filesystem>

#include "Renderer.h"

#include "imgui/imgui.h"

namespace test {
namespace fs = std::filesystem;

const std::string TestTexture2D::m_Title("2D Texture Test");

const float TestTexture2D::m_Vertices[2 * 4]{
    -1.0f, -1.0f, 
     1.0f, -1.0f,
     1.0f,  1.0f,
    -1.0f,  1.0f
};
const unsigned int TestTexture2D::m_Triangles[2 * 3]{
    0, 2, 1,
    2, 0, 3
};

TestTexture2D::TestTexture2D(Renderer* renderer)
    : m_Renderer(renderer)
    , m_IB(nullptr), m_VB(nullptr), m_Texture(nullptr)
    , m_Shader("res/shaders/TestTexture2D.glsl")
    , m_Scale(0.75f)
{
    fs::path texturePath = fs::path("res/textures");
    std::cout << "Searching for textures in " << texturePath << std::endl;
    for (const auto& dir_entry : fs::directory_iterator(texturePath)){
        if (dir_entry.is_regular_file() && dir_entry.path().extension() == ".png"){
            std::cout << "  " << dir_entry.path() << std::endl;
            m_TextureFiles.push_back(std::string(dir_entry.path().string()));
        }
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_VBLayout.Push<float>(2); // 3D position

    m_IB = new IndexBuffer(m_Triangles, 3 * 2);
    m_VB = new VertexBuffer(m_Vertices, 4 * m_VBLayout.GetStride());
    m_VA.Bind();
    m_VA.AddBuffer(*m_VB, m_VBLayout);

    m_Shader.Bind();
    m_Shader.SetUniform<float>("u_Scale", 1, &m_Scale);
    m_Shader.SetUniform<int>("u_Texture", 0);
}

TestTexture2D::~TestTexture2D(){
    delete m_IB;
    delete m_VB;
    delete m_Texture;
}

void TestTexture2D::OnUpdate(float deltatime){
}

void TestTexture2D::OnRender(){
    if(m_Texture) m_Texture->Bind();
    m_Shader.SetUniform<int>("u_Texture", 0);
    m_Renderer->Draw(m_VA, *m_IB, m_Shader);
}

void TestTexture2D::OnImGUIRender(){
    for (std::string source : m_TextureFiles){
        if (m_Texture && source == m_Texture->GetFilePath()){
            ImGui::Text(source.c_str());
        } else if (ImGui::Button(source.c_str())){
            delete m_Texture;
            m_Texture = new Texture(source);
        }
    }
    if(ImGui::SliderFloat("Scale", &m_Scale, 0.1f, 2.0f))
        m_Shader.SetUniform<float>("u_Scale", 1, &m_Scale);
}

} // namespace test
