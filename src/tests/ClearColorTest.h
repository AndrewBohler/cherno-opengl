#ifndef CLEAR_COLOR_TEST_H
#define CLEAR_COLOR_TEST_H

#include "Test.h"

#include <string>

namespace test {
class TestClearColor : public Test
{
    std::string m_Title;
    float m_ClearColor[4];

public:
    TestClearColor();
    ~TestClearColor();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGUIRender() override;
    std::string GetTitle() override;
};

} // namespace test

#endif // CLEAR_COLOR_TEST_H
