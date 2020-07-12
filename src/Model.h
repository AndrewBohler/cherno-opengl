#ifndef MODELa_H
#define MODEL_H

#include "glew.h"

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class Model{
    static unsigned int constructed_model_count;
    unsigned int render_id;
    unsigned int shader_id;
    VertexArray* vao;
    VertexBuffer* vbo;
    VertexBufferLayout* vbo_layout;
    IndexBuffer* ibo;

    protected:
        void assignID();

    public:
        Model();
        ~Model();
        void Render();
};

#endif