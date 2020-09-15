#include "TestAnimatePlane.h"

#include <vector>

#include "imgui/imgui.h"

#include "IndexBuffer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace test {

/* This is intended to be drawn with multiple triangle strips */
ModelPlane::ModelPlane(int vertices_x_count, int vertices_y_count, Renderer* renderer)
    : m_VerticesWidth(vertices_x_count)
    , m_VerticesHeight(vertices_y_count)
    , m_Animate(false), m_Scale(1.0f), m_Magnitude(1.0f)
    , m_Shader("res/shaders/TestAnimatePlane.glsl")
    , m_Texture("res/textures/drew_blurry.png")
    , m_VA(new VertexArray()), m_VB(nullptr), m_IB(nullptr)
    , m_Renderer(renderer)
{
    if (vertices_x_count < 2 || vertices_y_count < 2) throw std::invalid_argument("must be at least 2x2");
    m_VA->Bind();
    m_VBLayout.Push<float>(2);
    GenerateVertexBuffer();
    m_VA->AddBuffer(*m_VB, m_VBLayout);
    GenerateIndexBuffer();
}

ModelPlane::~ModelPlane(){
    delete m_Vertices;
    delete m_VA;
    delete m_VB;
    delete m_IB;
}

void ModelPlane::GenerateVertexBuffer(){
    struct coord2D {
        float x;
        float y;
    };
    
    /* Assuming that VBLayout is static, just (x, y) coords */    
    if (m_VB) delete m_VB;
    int vertexCount = m_VerticesWidth * m_VerticesHeight;
    coord2D* vertices = new coord2D[vertexCount];

    // normalization variables
    float xDivisor = float(m_VerticesWidth - 1) / 2;
    float yDivisor = float(m_VerticesHeight - 1) / 2;
    
    // assign values normalized to +/- 1
    for (int row = 0; row < m_VerticesWidth; row++){
        for (int col = 0; col < m_VerticesHeight; col++)
        {
            vertices[row + col * m_VerticesWidth]
                = coord2D{(row / xDivisor) - 1.0f, (col / yDivisor) - 1.0f};
        }
    }

    // copy data to buffer and delete temp variable
    m_VB = new VertexBuffer((void*)vertices,
        vertexCount * m_VBLayout.GetStride());
    delete vertices;
}

void ModelPlane::GenerateIndexBuffer(){
    struct uiPair{
        unsigned int top;
        unsigned int bot;
    };
    
    const int width = m_VerticesWidth;
    const int height = m_VerticesHeight - 1; // strips are 2 verts tall
    const int indexPairCount = width * height; // this is actually only 1/2 count
    uiPair* indexPairs = new uiPair[indexPairCount];
    
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            indexPairs[x + y * width].top
                = (unsigned int)(x + y * width);
            indexPairs[x + y * width].bot
                = (unsigned int)(x + (y + 1) * width);
        }
    }

    unsigned int* indices = (unsigned int*)indexPairs;
    int indexCount = indexPairCount * 2;
    m_IB = new IndexBuffer(indices, indexCount, GL_TRIANGLE_STRIP);

    for (int row = 0; row < height; row++){
        m_IB->AddTriStrip(TriangleStrip{
            (unsigned int)(width * 2),
            (void*)(row * width * 2 * sizeof(float)) // unsure how sizeof(float) is correct...
        });
    }

    delete indices;
}

void ModelPlane::SetVertexDim(int x, int y){

    m_VerticesWidth = x;
    m_VerticesHeight = y;
    delete m_VA;
    m_VA = new VertexArray();
    GenerateVertexBuffer();
    GenerateIndexBuffer();
    m_VA->AddBuffer(*m_VB, m_VBLayout);
}

void ModelPlane::Render(){
    m_Texture.Bind();
    m_Shader.Bind();
    m_Shader.SetUniform<int>("u_Animate", m_Animate); // no bool uniform setter
    m_Shader.SetUniform<float>("u_Magnitude", m_Magnitude);
    m_Shader.SetUniform<float>("u_Scale", m_Scale);
    m_Shader.SetUniform<int>("u_Texture", 0);
    m_Shader.SetUniform<float>("u_Time", m_Time);
    m_Renderer->Draw(m_VA, m_IB, &m_Shader, GL_TRIANGLE_STRIP);
}


TestAnimatePlane::TestAnimatePlane(Renderer* renderer)
    : m_Title("Texture Plane Test"), m_Renderer(renderer)
    , m_Plane(10, 10, renderer)
{
    m_Plane.SetRenderer(renderer);
}

TestAnimatePlane::~TestAnimatePlane(){
    
}

void TestAnimatePlane::OnUpdate(float deltaTime){
    m_Plane.UpdateTime(deltaTime);
}

void TestAnimatePlane::OnRender(){
    m_Plane.Render();
}

void TestAnimatePlane::OnImGUIRender(){
    ImGui::SliderFloat("Scale", &m_Plane.m_Scale, 0.1f, 2.0f);
    ImGui::Checkbox("Animate", &m_Plane.m_Animate);
    if (m_Plane.m_Animate)
        ImGui::SliderFloat("Magnitude", &m_Plane.m_Magnitude, 0.0f, 10.0f);
}

} // namespace test
