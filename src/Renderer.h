#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) {std::cout << "ASSERTION error!" << std::endl; std::system("pause");}

class Camera {
    glm::vec3 m_Pos;
    glm::vec3 m_Focus;
    glm::mat4 m_Model;
    glm::mat4 m_View;
    glm::mat4 m_Proj;
    glm::mat4 m_MVP;

public:
    Camera(float x, float y, float z);
    ~Camera();
    void SetPosition(glm::vec3 pos){ m_Pos = pos; }
    void SetFocus(glm::vec3 focus){ m_Focus = focus; }
    void Translate(glm::vec3 translation){ m_Pos += translation; }
    void Rotate(glm::vec3 rotation){}
    void UpdateMVP(){ m_MVP = m_Proj * m_View * m_Model; } // column major so reverse order
    glm::mat4 MVP(){ return m_MVP; }
};

class Renderer{
    Camera m_Camera;

public:
    Renderer();
    void Clear();
    void Draw(
        const VertexArray& va,
        const IndexBuffer& ib,
        const Shader& shader,
        GLenum mode = GL_TRIANGLES
    ) const;
    void Draw(
        const VertexArray* va,
        const IndexBuffer* ib,
        const Shader* shader,
        GLenum mode = GL_TRIANGLES
    ) const;
    glm::mat4 GetMVP(){ return m_Camera.MVP(); }

private:
    void DrawTriangles(unsigned int count) const;
    void DrawTriangleStrips(std::vector<TriangleStrip>) const;
};

#endif