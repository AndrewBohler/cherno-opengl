#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <GL/glew.h>

#include "glm/glm.hpp"

struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader{
    const std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

    public:
        Shader(const std::string& filepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;
        unsigned int GetID();

        // set uniforms
        void SetUniform4f(const std::string&, float v0, float v1, float v2, float v3);
        void SetUniformMat4f(const std::string&, int count, const glm::mat4 matrix);
        
        template<typename T>
        void SetUniform(const std::string&, unsigned int count, T* value);
        template<typename T> void SetUniform(const std::string&, T value);

    private:
        ShaderProgramSource ParseShader(const std::string& filepath);
        unsigned int CompileShader(unsigned int type, const std::string& source);
        unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        int GetUniformLocation(const std::string& name);
};

#endif