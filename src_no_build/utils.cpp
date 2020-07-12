#include "glew.h"
#include "utils.h"
#include <vector>
#include <iostream>
#include <cstring>

// unsigned int Buffer::m_id{0};
// unsigned int Buffer::m_type{0};
// unsigned int Buffer::m_size{0};
// unsigned int Buffer::m_attributes{0};
// void * Buffer::m_data{nullptr};

Buffer::Buffer(GLenum type, void * data, unsigned int count){
    m_type = type;
    switch (type){
        case GL_UNSIGNED_INT: m_data = new unsigned int[count]; break;
        case GL_INT         : m_data = new int[count]; break;
        case GL_FLOAT       : m_data = new float[count]; break;
        case GL_DOUBLE      : m_data = new double[count]; break;
        default             : typedef void d_type;
    }
    m_size = size;
    m_data = new d_type[size];
    std::memcpy()
    glGenBuffers(1, &m_id);

}

Buffer::~Buffer(){
    glDeleteBuffers(1, &m_id);
    delete[] m_data;
}

int Buffer::load(void * data, unsigned int size){
    if (!m_id) return 1;
    
}
