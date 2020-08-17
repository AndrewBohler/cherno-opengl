#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray(){
    glGenVertexArrays(1, &m_RendererID);
    glBindVertexArray(m_RendererID);
}

VertexArray::~VertexArray(){
    glDeleteVertexArrays(1, &m_RendererID);
    std::cout << "[VertexArray " << m_RendererID << "] destroyed!" << std::endl;
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout){
    std::cout << "[VertexArray " << m_RendererID << "] binding vertex buffer" << std::endl;
    vb.Bind();
    std::cout << "[VertexArray " << m_RendererID << "] getting layout elements" << std::endl;
    const auto elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++){
        const auto& element = elements[i];
        std::cout << "[VertexArray " << m_RendererID << "] enabling vertex attribute " << i << std::endl;
        glEnableVertexAttribArray(i);
        std::cout << "[VertexArray " << m_RendererID << "] setting up vertex attribute pointer" << std::endl;
        glVertexAttribPointer(i, element.count, element.type,
            element.normalized, layout.GetStride(), (const void*)offset);
        std::cout << "[VertexArray " << m_RendererID << "] incrementing offset" << std::endl;
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const{
    glBindVertexArray(m_RendererID);

}

void VertexArray::Unbind() const{
    glBindVertexArray(0);
}