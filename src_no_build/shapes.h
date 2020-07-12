#ifndef SHAPES_H
#define SHAPES_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Shape {
    Shape();
    ~Shape();
};

class Cube {
    static const char shape_name[];

    // float position[3]{0};
    static const int vertex_elem_count     = 3 + 3; // xyz + rgb
    static const int vertex_count          = 8;
    static const int vertex_array_length   = vertex_elem_count * vertex_count; // 8 vertices
    static const int vertices_stride       = sizeof(float) * vertex_elem_count;
    static const int vertices_total_size   = sizeof(float) * vertex_array_length;

    static const int triangle_elem_count   = 3; // 3 vertex indices
    static const int triangles_count       = 2 * 6; // 2 triangles per face, 6 faces
    static const int triangle_array_length = triangle_elem_count * triangles_count;
    static const int triangles_stride      = sizeof(unsigned int) * triangle_elem_count;
    static const int triangles_total_size  = sizeof(unsigned int) * triangle_elem_count * triangles_count;

    float *vertices;

    static const float default_vertices[vertex_array_length];
    static const unsigned int triangles[triangle_array_length];

    unsigned int vertex_array = 0;
    VertexBuffer vertex_buffer;
    IndexBuffer index_buffer;
    unsigned int shader = 0;
    
    static unsigned int constructed_count;

    void printDebugPrefix();
    // void updateVertexBuffer();

    public:
        unsigned int id;

        Cube();
        ~Cube();

        void translate(float x, float y, float z);
        void setShader(unsigned int shader_id);
        void render();
        void debugInfo();
};


class Sphere {
    static const char shape_name[];
    static const int sections = 3;
    static const float sectionAngle;
    // float position[3]{0};

    /* vertex buffer */
    static const int vb_elem_count = 3 + 3; // xyz + rgb
    static const int vb_count      = 8;
    static const int vb_length     = vb_elem_count * vb_count; // 8 vertices
    static const int vb_stride     = sizeof(float) * vb_elem_count;
    static const int vb_size       = sizeof(float) * vb_length;

    /* index buffer */
    static const int ib_elem_count = 3; // 3 vertex indices
    static const int ib_count      = 2 * 6; // 2 triangles per face, 6 faces
    static const int ib_length     = ib_elem_count * ib_count;
    static const int ib_stride     = sizeof(unsigned int) * ib_elem_count;
    static const int ib_total_size = sizeof(unsigned int) * ib_elem_count * ib_count;

    static float * vb_default_data_arr;
    static unsigned int * ib_data_arr;

    float * vb_data_arr;

    unsigned int vertex_buffer = 0;
    unsigned int shader = 0;
    unsigned int id;
    static unsigned int index_buffer;
    static unsigned int constructed_count;

    void createVertexBuffer();
    void genDefaultVertices();
    void printDebugPrefix();

    public:
        Sphere();
        ~Sphere();
        void setShader(unsigned int shader_id);
        void render();
};


#endif
