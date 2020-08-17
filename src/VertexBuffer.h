#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <GL/glew.h>

class VertexBuffer{
    unsigned int m_RendererID;
    unsigned int m_Size;

    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
        void Update(const void* data, unsigned int size, const int offset = 0);
        unsigned int Size();
        void GetData(void* destination, unsigned int size);
};

#endif
