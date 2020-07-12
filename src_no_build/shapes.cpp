#include "shapes.h"
#include "glew.h"
#include <iostream>
#include <cstring> // memcpy comes from here
#include <math.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"



#ifndef PI
#define PI 3.14159265359
#endif

#define DEBUG_SHAPES true



/* Cube */

const char Cube::shape_name[] = "Cube";
/* note that static variables (declared in the header file) default to 0 anyways */
IndexBuffer Cube::index_buffer{0};
VetexBuffer Cube::constructed_count{0};

// void Cube::updateVertexBuffer(){
//     glBindVertexArray(vertex_array);
//     glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//     glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_total_size, vertices);
//     if (id == 1){
//         float data;
//         glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float), &data);
//         std::cout << "first vertex x = " << data << std::endl;
//     }
// }

void Cube::printDebugPrefix(){
    std::cout << "[" << shape_name << " " << id << "] ";
}

void Cube::setShader(unsigned int shader_id){
    if (DEBUG_SHAPES){
        printDebugPrefix();
        std::cout << "setting shader to " << shader_id << std::endl;
    }
    shader = shader_id;
}

Cube::Cube()
    : vertex_buffer(&default_vertices, vertices_total_size)
    , index_buffer(triangles, triangle_array_length)
    , id(constructed_count++)
{
    // id = constructed_count++;
    vertices = new float[vertex_array_length];
    std::memcpy(vertices, default_vertices, vertices_total_size);

    if (DEBUG_SHAPES) {
        printDebugPrefix();
        std::cout << "created!" << std::endl;
    }
}

Cube::~Cube(){
    if (DEBUG_SHAPES) {
        printDebugPrefix();
        std::cout << "destructor called!" << std::endl;
    }
    delete[] vertices;
}

void Cube::translate(float tx, float ty, float tz){
    if (id == 1) std::cout << std::endl; // debug
    
    for (int i = 0; i < vertex_count; i++){
        if (id == 0 && i == 0){ // debug
            std::cout << "(" << vertices[i * 6] << ", " << vertices[i* 6 + 1] << ", " << vertices[i * 6 + 2] << "), ";
            std::cout << " + " << "(" << tx << ", " << ty << ", " << tz << ") = ";
        }

        if (id > 98){
            vertices[i * vertex_elem_count + 0] += -(1.0f / (float)(id / 10 + 1));//tx;
            vertices[i * vertex_elem_count + 1] += -(1.0f / (float)(id / 10 + 1));//ty;
            vertices[i * vertex_elem_count + 2] += 0.0f;//tz;
        }

        if (id == 0 && i == 0){ // debug
            std::cout << "(" << vertices[i * 6] << ", " << vertices[i * 6 + 1] << ", " << vertices[i * 6 + 2] << "), " << std::endl;
        }
    }

    updateVertexBuffer();
}

void Cube::render(){
    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glUseProgram(shader);
    glUniform1f(glGetUniformLocation(shader, "id"), (float)id);
    glDrawElements(GL_TRIANGLES, triangle_array_length, GL_UNSIGNED_INT, nullptr);
}

void Cube::debugInfo(){
    printDebugPrefix();
    std::cout << "*** debug info ***" << std::endl;
    std::cout << "\t" << "id              :" << id << std::endl;
    std::cout << "\t" << "default_vertices:" << default_vertices << std::endl;
    std::cout << "\t" << "vertex_buffer   :" << vertex_buffer << std::endl;
    std::cout << "\t" << "index_buffer    :" << index_buffer << std::endl;
    std::cout << "\t" << "shader          :" << shader << std::endl;
    std::cout << "\t" << "vertices adress :" << vertices << std::endl;
    std::cout << "\t" << "indices adress  :" << triangles << std::endl;
}


const float Cube::default_vertices[vertex_array_length]{
    /* (x,     y,     z)     (r,     g,     b) */
     1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f, // 0
     1.0f, -1.0f,  1.0f,   1.0f,  0.0f,  0.0f, // 1
    -1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f, // 2
    -1.0f, -1.0f,  1.0f,   1.0f,  0.0f,  0.0f, // 3

     1.0f,  1.0f, -1.0f,   0.0f,  0.0f,  1.0f, // 4
     1.0f, -1.0f, -1.0f,   0.0f,  0.0f,  1.0f, // 5
    -1.0f,  1.0f, -1.0f,   0.0f,  0.0f,  1.0f, // 6
    -1.0f, -1.0f, -1.0f,   0.0f,  0.0f,  1.0f, // 7
};

const unsigned int Cube::triangles[triangle_array_length]{
    0, 1, 2,   3, 2, 1, // front
    6, 5, 4,   5, 6, 7, // back
    6, 2, 3,   3, 7, 6, // left
    4, 5, 0,   1, 0, 5, // right
    0, 2, 4,   6, 4, 2, // top
    1, 5, 3,   7, 3, 5, // bottom
};



/* Sphere */

const char Sphere::shape_name[] = "Sphere";
float * Sphere::vb_default_data_arr = nullptr;
unsigned int * Sphere::ib_data_arr = nullptr;
unsigned int Sphere::constructed_count{0};
unsigned int Sphere::index_buffer{0};
const float Sphere::sectionAngle = 2 * PI / sections;

void Sphere::printDebugPrefix(){
    std::cout << "[" << shape_name << " " << id << "] ";
}

void Sphere::createVertexBuffer(){
    if (vertex_buffer) {
        printDebugPrefix();
        std::cout << "vertex buffer already exists!" << std::endl;
        return;
    } else if (DEBUG_SHAPES) {
        printDebugPrefix();
        std::cout << "creating vertex buffer" << std::flush;
    }

    if (DEBUG_SHAPES) std::cout << "0" << std::flush;
    glGenBuffers(1, &vertex_buffer);
    if (DEBUG_SHAPES) std::cout << "1" << std::flush;
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    if (DEBUG_SHAPES) std::cout << "2" << std::flush;
    glBufferData(GL_ARRAY_BUFFER, vb_size, vb_data_arr, GL_STATIC_DRAW);
    if (DEBUG_SHAPES) std::cout << "3" << std::flush;
    glEnableVertexAttribArray(0);
    if (DEBUG_SHAPES) std::cout << "4" << std::flush;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vb_stride, (void *)0);
    if (DEBUG_SHAPES) std::cout << "5" << std::flush;
    glEnableVertexAttribArray(1);
    if (DEBUG_SHAPES) std::cout << "6" << std::flush;
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vb_stride, (void *)(sizeof(float) * 3));
    if (DEBUG_SHAPES) std::cout << "7" << std::endl;
};

void Sphere::setShader(unsigned int shader_id){
    if (DEBUG_SHAPES) {
        printDebugPrefix();
        std::cout << "setting shader to" << shader_id << std::endl;
    }
    shader = shader_id;
};

Sphere::Sphere(){
    id = constructed_count++;
    if (vb_default_data_arr == nullptr) genDefaultVertices();
    vb_data_arr = new float[vb_length];
    std::memcpy(vb_data_arr, vb_default_data_arr, vb_size);

    if (DEBUG_SHAPES) {
        printDebugPrefix();
        std::cout << "calling createVertexBuffer()" << std::endl;
    }
    createVertexBuffer();

    if (ib_data_arr == nullptr){
        if (DEBUG_SHAPES) {
            printDebugPrefix();
            std::cout << "creating index array for " << shape_name << std::endl;
        }
        /* create indices */
        ib_data_arr = new unsigned int[ib_length];
        for (int i = 0; i < sections; i++){
            for (int j = 0; j < sections; j++){
                /* note that the modulo keeps us in bounds */
                /* first triangle */
                ib_data_arr[i * sections * 6 + (j * 6) + 0] =  i      * sections + j     % (sections * sections);
                ib_data_arr[i * sections * 6 + (j * 6) + 1] = (i + 1) * sections + j     % (sections * sections);
                ib_data_arr[i * sections * 6 + (j * 6) + 2] =  i      * sections + j + 1 % (sections * sections);

                /* second triangle */
                ib_data_arr[i * sections * 6 + (j * 6) + 3] =  i      * sections + j + 1 % (sections * sections);
                ib_data_arr[i * sections * 6 + (j * 6) + 4] = (i + 1) * sections + j     % (sections * sections);
                ib_data_arr[i * sections * 6 + (j * 6) + 5] =  i      * sections + j + 1 % (sections * sections);

            }
        }
    }

    if (!index_buffer) {
        if (DEBUG_SHAPES) {
            printDebugPrefix();
            std::cout << "creating index buffer for " << shape_name << std::endl;
        }
        glGenBuffers(1, &index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ib_length, ib_data_arr, GL_STATIC_DRAW);
    }
    if (DEBUG_SHAPES){
        printDebugPrefix();
        std::cout << "created!" << std::endl;
    }
};

Sphere::~Sphere(){
    if (DEBUG_SHAPES){
        printDebugPrefix();
        std::cout << "calling destructor" << std::endl;
    }
    delete[] vb_data_arr;
}

void Sphere::render(){
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glUseProgram(shader);
    glUniform1f(glGetUniformLocation(shader, "id"), (float)id);
    glDrawElements(GL_TRIANGLES, ib_length, GL_UNSIGNED_INT, nullptr);
}

void Sphere::genDefaultVertices(){
    if (vb_default_data_arr != nullptr){
        std::cout << "error genDefaultVertices(): Sphere already has default vertices" << std::endl;
        return;
    } else {
        vb_default_data_arr = new float[vb_length];
    }

    /* create vertices */
    for (int i = 0; i < sections; i++){
        for (int j = 0; j < sections; j++){
            float z_rot = sectionAngle * i;
            float y_rot = sectionAngle * j;

            /* position */
            vb_default_data_arr[i * sections * vb_elem_count + j * vb_elem_count + 0] = cos(z_rot) - sin(y_rot);
            vb_default_data_arr[i * sections * vb_elem_count + j * vb_elem_count + 1] = sin(z_rot);
            vb_default_data_arr[i * sections * vb_elem_count + j * vb_elem_count + 2] = sin(y_rot);

            /* color */
            vb_default_data_arr[i * sections * vb_elem_count + j * vb_elem_count + 3] = j % 2 / 2 + 0.25;
            vb_default_data_arr[i * sections * vb_elem_count + j * vb_elem_count + 4] = j % 2 / 2 + 0.25;
            vb_default_data_arr[i * sections * vb_elem_count + j * vb_elem_count + 5] = j % 2 / 2 + 0.25;
        }
    }

}
