#ifndef TEST_H
#define TEST_H

#include <GL/glew.h>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "Renderer.h"
#include "Texture.h"

namespace test {
class Test {
    static const std::string m_Title;
    Renderer* m_Renderer;

public:
    Test() {}
    virtual ~Test() {}

    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnImGUIRender() {}
    virtual std::string GetTitle() { return m_Title; }
};

class TestMenu : public Test
{
    static const std::string m_Title;
    Renderer* m_Renderer;
    Texture m_Texture;

public:
    TestMenu(Test*& currentTest_ptr, Renderer* renderer_ptr);
    
    void OnImGUIRender() override;
    std::string GetTitle() override { return m_Title; }

    template<typename T>
    void RegisterTest(const std::string& name)
    {
        std::cout << "Registering test '" << name << "'" << std::endl;
        m_Tests.push_back(std::make_pair(name, [](Renderer* renderer_ptr){ return new T(renderer_ptr); }));
    }

private:
    Test*& m_CurrentTest;
    std::vector<std::pair<std::string, std::function<Test*(Renderer*)>>> m_Tests;
};

} // namespace test

#endif // TEST_H