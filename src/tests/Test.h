#ifndef TEST_H
#define TEST_H

#include <GL/glew.h>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace test{
class Test{
    public:
        Test() {}
        virtual ~Test() {}

        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGUIRender() {}
        virtual ::std::string GetTitle() {}
};

class TestMenu : public Test
{
public:
    TestMenu(Test*& currentTest_ptr);
    
    void OnImGUIRender() override;

    template<typename T>
    void RegisterTest(const std::string& name)
    {
        std::cout << "Registering test " << name << std::endl;
        m_Tests.push_back(std::make_pair(name, [](){ return new T(); }));
    }
private:
    Test*& m_CurrentTest;
    std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
};

} // namespace test

#endif // TEST_H