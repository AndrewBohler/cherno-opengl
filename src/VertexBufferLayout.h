#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include "glew.h"
#include <vector>
#include <iostream>

struct VertexBufferElement{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type){
        switch (type){
            case GL_UNSIGNED_INT : return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
            case GL_FLOAT        : return sizeof(GLfloat);
        }
        std::cout << "Warning: VertexBufferElement type unknown" << std::endl;
        return 0;
    }
};

class VertexBufferLayout{
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

    public:
        VertexBufferLayout()
            : m_Stride(0)
        {

        }
        template<typename T>
        void Push(unsigned int count);

        inline const std::vector<VertexBufferElement>& GetElements() const;
        inline unsigned int GetStride() const {return m_Stride;}
};


template<typename T>
inline void VertexBufferLayout::Push(unsigned int count){
    static_assert(sizeof(T) == -1, "Can only push types that are specialized");
}

template<>
inline void VertexBufferLayout::Push<float>(unsigned int count){
    m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_Stride += sizeof(GLfloat) * count;
}

template<>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count){
    m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_Stride += sizeof(GLuint) * count;
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count){
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_Stride += sizeof(GLubyte) * count;
}

inline std::vector<VertexBufferElement> const& VertexBufferLayout::GetElements() const {return m_Elements;}

#endif