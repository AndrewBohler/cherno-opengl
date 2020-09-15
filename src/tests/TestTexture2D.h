#ifndef TEST_TEXTURE2D_H
#define TEST_TEXTURE2D_H

#include "Test.h"

#include <filesystem>
#include <string>
#include <vector>

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace test {
namespace fs = std::filesystem;

class TestTexture2D : public Test
{
    static const std::string m_Title;
    static const float m_Vertices[];
    static const unsigned int m_Triangles[];
    std::vector<std::string> m_TextureFiles;
    IndexBuffer* m_IB;
    VertexArray m_VA;
    VertexBuffer* m_VB;
    VertexBufferLayout m_VBLayout;
    Renderer* m_Renderer;
    Shader m_Shader;
    Texture* m_Texture;

    float m_Scale;

public:
    TestTexture2D(Renderer*);
    ~TestTexture2D();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGUIRender() override;
    std::string GetTitle() override { return m_Title; }
};

} // namespace test

#endif // TEST_TEXTURE2D_H
