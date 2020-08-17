#ifndef CLEAR_COLOR_TEST_H
#define CLEAR_COLOR_TEST_H

#include "Test.h"

namespace test {
    class TestClearColor : public Test
    {
    public:
        TestClearColor();
        ~TestClearColor();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGUIRender() override;
    };
}

#endif // CLEAR_COLOR_TEST_H