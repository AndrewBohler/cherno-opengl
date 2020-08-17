#ifndef TEST_H
#define TEST_H

#include <GL/glew.h>

namespace test{
    class Test{
        public:
            Test() {}
            virtual ~Test() {}

            virtual void OnUpdate() {}
            virtual void OnRender() {}
            virtual void OnImGUIRender() {}
    };
}

#endif // TEST_H