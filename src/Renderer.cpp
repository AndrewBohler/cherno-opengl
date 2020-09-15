#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(float x, float y, float z)
    : m_Pos(x, y, z)
{
    m_Focus = m_Pos + glm::vec3(0.0f, 0.0f, 1.0f);
    m_Model = glm::mat4();
    m_View = glm::lookAt(m_Pos, m_Focus, glm::vec3(0.0f, 1.0f, 0.0f));
    m_Proj = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 100.0f);
    m_MVP = m_Proj * m_View * m_Model;
}

Camera::~Camera(){

}

Renderer::Renderer()
    : m_Camera(Camera(0.0f, 0.0f, 0.0f))
{

}

void Renderer::Clear(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(
    const VertexArray& va,
    const IndexBuffer& ib,
    const Shader& shader,
    GLenum mode
) const {
    va.Bind();
    ib.Bind();
    shader.Bind();
    glDrawElements(mode, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(
    const VertexArray* va,
    const IndexBuffer* ib,
    const Shader* shader,
    GLenum mode
) const {
    va->Bind();
    ib->Bind();
    shader->Bind();
    
    switch (mode){
        case GL_TRIANGLES: DrawTriangles(ib->GetCount()); return;
        case GL_TRIANGLE_STRIP: DrawTriangleStrips(ib->GetTriStrips()); return;
        default: throw std::invalid_argument("Unimplimented Mode");
    }
}

void Renderer::DrawTriangles(unsigned int count) const {
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawTriangleStrips(std::vector<TriangleStrip> strips) const {
    for (auto& ts : strips) glDrawElements(
        GL_TRIANGLE_STRIP, ts.count, GL_UNSIGNED_INT, (void*)ts.offset);
}
