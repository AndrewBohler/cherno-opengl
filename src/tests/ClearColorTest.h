#ifndef CLEAR_COLOR_TEST_H
#define CLEAR_COLOR_TEST_H

#include "Test.h"

#include <string>

#include "Renderer.h"

namespace test {
class TestClearColor : public Test
{
    static const std::string m_Title;
    float m_ClearColor[4];
    Renderer* m_Renderer;

public:
    TestClearColor(Renderer*);
    ~TestClearColor();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGUIRender() override;
    std::string GetTitle() override { return m_Title; }
};

} // namespace test

#endif // CLEAR_COLOR_TEST_H
