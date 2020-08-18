#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"

// #include "Renderer.h"


Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader(){

}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);
    const char * src = source.c_str(); // needs to be a C string aka const char[]
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        
        // alloca() dynamically allocates memory on the stack
        // supposedly this is a dangerous operation
        // best practice: don't do this with a large amount of memory
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile ";
        switch (type){
            case GL_VERTEX_SHADER           : std::cout << "vertex"; break;
            case GL_TESS_CONTROL_SHADER     : std::cout << "tesselation control"; break;
            case GL_TESS_EVALUATION_SHADER  : std::cout << "tesselation evaluation"; break;
            case GL_GEOMETRY_SHADER         : std::cout << "geometry"; break;
            case GL_FRAGMENT_SHADER         : std::cout << "fragment"; break;
            case GL_COMPUTE_SHADER          : std::cout << "compute"; break;
            default                         : std::cout << "unknown type";
        } std::cout << " shader!" << std::endl;
            
        std::cout << message << std::endl;
        
        // delete the shader because it didn't compile so we don't really need it
        glDeleteShader(id);

        // delete program instead because ??? (later video)
        // glDeleteProgram(/*program?*/);
        return 0;
    }
    return id;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath){
    std::ifstream stream(filepath);

    enum class ShaderType{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
            else{
                ss[(int)type] << line << "\n";
            }
        }
        else{
            ss[(int)type] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    int status = GL_FALSE;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE){
        std::cout << "[Shader " << program << "] error: validation failed for " << m_FilePath << std::endl;
    } else {
        std::cout << "[Shader " << program << "] program validated" << std::endl;
    }

    // shaders are now in the program, free the memory
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const { glUseProgram(m_RendererID); }
void Shader::Unbind() const { glUseProgram(0); }
unsigned int Shader::GetID() { return m_RendererID; }

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3){
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, int count, const glm::mat4 matrix){
    if (!count) return;
    int location = GetUniformLocation(name);
    if (location == -1) return;
    glUniformMatrix4fv(location, count, GL_FALSE, &matrix[0][0]);
}

template<typename T>
void Shader::SetUniform(const std::string& name, unsigned int count, T* values){
    GetUniformLocation(name);
    std::cout << "SetUniform error: invalid type, template specialization doesn't exist" << std::endl;
    return;
}

template<>
void Shader::SetUniform<float>(const std::string& name, unsigned int count, float* value){
    if (!count) return;
    int location = GetUniformLocation(name);
    if (location == -1) return;
    
    switch(count){
        case 1  : glUniform1f(location, *value); return;
        case 2  : glUniform2f(location, value[0], value[1]); return;
        case 3  : glUniform3f(location, value[0], value[1], value[2]); return;
        case 4  : glUniform4f(location, value[0], value[1], value[2], value[3]); return;
        default : std::cout << "cannot set uniform, 'count' should be 1, 2, 3, or 4, not " << count << std::endl; return;
    }
}

template<>
void Shader::SetUniform<unsigned int>(const std::string& name, unsigned int count, unsigned int* value){
    if (!count) return;
    int location = GetUniformLocation(name);
    if (location  == -1) return;
    switch(count){
        case 1  : glUniform1ui(location, *value); return;
        case 2  : glUniform2ui(location, value[0], value[1]); return;
        case 3  : glUniform3ui(location, value[0], value[1], value[2]); return;
        case 4  : glUniform4ui(location, value[0], value[1], value[2], value[3]); return;
        default : std::cout << "cannot set uniform, 'count' should be 1, 2, 3, or 4, not " << count << std::endl; return;
    }
}

template<>
void Shader::SetUniform<int>(const std::string& name, unsigned int count, int* value){
    if (!count) return;
    int location = GetUniformLocation(name);
    if (location  == -1) return;
    switch(count){
        case 1  : glUniform1i(location, *value); return;
        case 2  : glUniform2i(location, value[0], value[1]); return;
        case 3  : glUniform3i(location, value[0], value[1], value[2]); return;
        case 4  : glUniform4i(location, value[0], value[1], value[2], value[3]); return;
        default : std::cout << "cannot set uniform, 'count' should be 1, 2, 3, or 4, not " << count << std::endl; return;
    }
}

int Shader::GetUniformLocation(const std::string& name){
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
        
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cout << "[Shader " << m_RendererID << "] Warning: uniform '"
            << name << "' does not exist in " << m_FilePath << std::endl;
    
    m_UniformLocationCache[name] = location;
    return location;
}

