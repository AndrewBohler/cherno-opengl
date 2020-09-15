#pragma once

#include "Test.h"

#include "IndexBuffer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace test {


class WaterModel {
    float   m_Time;

    int     m_VerticesWidth;
    int     m_VerticesHeight;
    float*  m_Vertices;
    
    std::vector<int> m_Indices;

    IndexBuffer*        m_IB;
    Renderer*           m_Renderer;
    Shader              m_Shader;
    Texture             m_Texture;
    VertexArray*        m_VA;
    VertexBuffer*       m_VB;
    VertexBufferLayout  m_VBLayout;

    void GenerateVertexBuffer();
    void GenerateIndexBuffer();

public:

    WaterModel(int, int, Renderer*);
    ~WaterModel();

    bool  m_Animate;
    float m_Magnitude;
    float m_Scale;

    float m_SheenColor[4];
    float m_SheenHeight;

    float m_ShadowColor[4];
    float m_ShadowHeight;
    
    void SetAnimate(bool state) { m_Animate = state; };
    void SetRenderer(Renderer* renderer) { m_Renderer = renderer; };
    void SetVertexDim(int x, int y);
    void UpdateTime(float time) { m_Time = time; }
    void Render();

    int GetVertWidth() { return m_VerticesWidth; }
    int GetVertHeight() { return m_VerticesHeight; }
};

class TestWaterTexture : public Test {
    std::string m_Title;
    Renderer* m_Renderer;
    
public:
    TestWaterTexture(Renderer*);
    ~TestWaterTexture();
    void OnUpdate(float deltaTime) override;
    void OnImGUIRender() override;
    void OnRender() override;
    std::string GetTitle() override { return m_Title; };

private:
    WaterModel m_Model;

};

} // namespace test