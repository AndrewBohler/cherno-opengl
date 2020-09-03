#pragma once
#include "Test.h"

#include "imgui/imgui.h"

namespace test {

TestMenu::TestMenu(Test*& currentTest_ptr)
    : m_CurrentTest(currentTest_ptr)
{

}

void TestMenu::OnImGUIRender(){
    for (auto& test : m_Tests)
    {
        if (ImGui::Button(test.first.c_str()))
            m_CurrentTest = test.second();
    }
}

} // namespace test