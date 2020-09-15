#ifndef TEST_TEXTURE3D_H
#define TEST_TEXTURE3D_H

#include "Test.h"

#include <string>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace test {

class TestTexture3D : public Test
{
    static const std::string m_Title;
    static const float m_Vertices[(3 + 3 + 2) * 8];
    static const unsigned int m_Triangles[3 * 2 * 6];
    IndexBuffer* m_IB;
    VertexArray m_VA;
    VertexBuffer* m_VB;
    VertexBufferLayout m_VBLayout;
    Renderer* m_Renderer;
    Shader m_Shader;
    Texture m_Texture;


public:
    TestTexture3D(Renderer*);
    ~TestTexture3D();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGUIRender() override;
    std::string GetTitle() override { return m_Title; }
};

} // namespace test

#endif // TEST_TEXTURE3D_H
