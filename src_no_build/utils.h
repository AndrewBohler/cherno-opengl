#ifndef UTILS_h
#define UTILS_H

#include "glew.h"
#include <vector>

struct AttributeLayout{
    GLenum type;
    unsigned int count;
    size_t size;

};

class Buffer{
    private:
        unsigned int m_id{0};
        unsigned int m_type{0};
        size_t m_size{0};
        unsigned int m_attribute_count{0};
        AttributeLayout * m_attributes{nullptr};
        void * m_data{nullptr};

    public:
        Buffer(GLenum, void *, unsigned int);
        ~Buffer();
        int load(void *, unsigned int);
        int push();
};

struct VAO{
    private:
        Buffer vertex_buffer;
        unsigned int index_buffer;

    public:
        VAO();
};

class Renderer{
    std::vector<unsigned int> shaders;
    std::vector<unsigned int> VAO_vec;
    std::vector<unsigned int> 
};



#endif