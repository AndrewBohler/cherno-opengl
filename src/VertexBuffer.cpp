#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    : m_Size(size)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    std::cout << "[VertexBuffer " << m_RendererID << "] created!" << std::endl;
}

VertexBuffer::~VertexBuffer(){
    glDeleteBuffers(1, &m_RendererID);
    std::cout << "[VertexBuffer " << m_RendererID << "] destroyed!" << std::endl;
}

void VertexBuffer::Bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Update(const void* data, unsigned int size, const int offset){
    if (size + (unsigned int)offset > m_Size){
        std::cout << "[VertexBuffer] error: could not update, size of data + offset ("
            << size + offset << ") is greater than buffer size ("
            << m_Size << ")" << std::endl;
            return;
    }
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

unsigned int VertexBuffer::Size() {return m_Size;}

void VertexBuffer::GetData(void* destination, unsigned int size){
    if (size != m_Size){
        std::cout << "cannot copy buffer data, destination size (";
        std::cout << size << ") != buffer size (" << m_Size << ")";
        std::cout << std::endl;
        return;
    }
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, m_Size, destination);
}