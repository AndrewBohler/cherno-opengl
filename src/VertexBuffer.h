#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glew.h>

class VertexBuffer{
    unsigned int m_RendererID;

    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
};

#endif
