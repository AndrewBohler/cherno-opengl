#include "ClearColorTest.h"

#include "Renderer.h"

#include "imgui/imgui.h"

namespace test {

const std::string TestClearColor::m_Title("Clear Color Test");

TestClearColor::TestClearColor(Renderer* renderer)
    : m_ClearColor{0.0f, 0.0f, 0.0f, 0.0f}
    , m_Renderer(renderer)
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
    ImGui::ColorEdit4("Clear Color", m_ClearColor);
}

} // namespace test
