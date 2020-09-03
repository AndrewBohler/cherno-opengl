#include "ClearColorTest.h"

#include "Renderer.h"

#include "imgui/imgui.h"

namespace test {

TestClearColor::TestClearColor()
    : m_ClearColor{0.0f, 0.0f, 0.0f, 0.0f}
    , m_Title("Clear Color Test")
{

}

TestClearColor::~TestClearColor(){

}

void TestClearColor::OnUpdate(float deltatime){
    
}

void TestClearColor::OnRender(){
    glClearColor(m_ClearColor[0], m_ClearColor[1],
        m_ClearColor[2], m_ClearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void TestClearColor::OnImGUIRender(){
    ImGui::Begin(m_Title.c_str());
    ImGui::ColorEdit4("Clear Color", m_ClearColor);
    ImGui::End();
}

std::string TestClearColor::GetTitle(){
    return m_Title;
}

} // namespace test
