#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "glew.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray{
    unsigned int m_RendererID;

    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
        void Bind() const;
        void Unbind() const;
};

#endif