#include "Test.h"

#include "imgui/imgui.h"

#include "Renderer.h"

namespace test {
const std::string Test::m_Title("Test ABS");
const std::string TestMenu::m_Title("Test Menu");

TestMenu::TestMenu(Test*& currentTest_ptr, Renderer* renderer_ptr)
    : m_CurrentTest(currentTest_ptr)
    , m_Renderer(renderer_ptr)
    , m_Texture("res/textures/test_texture.png")
{
    glEnable(GL_TEXTURE_2D);
}

void TestMenu::OnImGUIRender(){
    for (auto& test : m_Tests)
    {
        if (ImGui::Button(test.first.c_str())){
            m_CurrentTest = test.second(m_Renderer);
        }
    }
    ImGui::Begin("Important Message");
    ImGui::Image((ImTextureID)m_Texture.GetID(),
        ImVec2((float)m_Texture.GetWidth(), (float)m_Texture.GetHeight()));
    ImGui::End();
}

} // namespace test