#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include "glew.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) {std::cout << "ASSERTION error!" << std::endl; std::system("pause");}

class Renderer{

    public:
        void Clear();
        void Draw(
            const VertexArray& va,
            const IndexBuffer& ib,
            const Shader& shader
        ) const;
};

#endif