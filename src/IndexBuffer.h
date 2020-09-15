#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <GL/glew.h>
#include <vector>

struct TriangleStrip{
    unsigned int count;
    void* offset;
};

class IndexBuffer{
    unsigned int m_RendererID;
    unsigned int m_Count;
    std::vector<TriangleStrip> m_TriangleStrips;

    public:
        IndexBuffer(const unsigned int* data, unsigned int count, GLenum renderMode = GL_TRIANGLES);
        ~IndexBuffer();

        GLenum m_RenderMode;

        void Bind() const;
        void Unbind() const;
        inline unsigned int GetCount() const { return m_Count; }
        void AddTriStrip(TriangleStrip ts) { m_TriangleStrips.push_back(ts); }
        const std::vector<TriangleStrip> GetTriStrips() const { return m_TriangleStrips; }
};

#endif // INDEX_BUFFER_H
