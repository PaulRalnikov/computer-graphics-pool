#include "shader_program.h"
#include <fstream>
#include <iostream>

static std::string read_shader(std::string path)
{
    std::ifstream in(path);
    std::string line;
    std::string res;
    while (std::getline(in, line))
    {
        res += line + "\n";
    }
    return res;
}

static GLuint create_shader(GLenum type, const char *source)
{
    GLuint result = glCreateShader(type);
    glShaderSource(result, 1, &source, nullptr);
    glCompileShader(result);
    GLint status;
    glGetShaderiv(result, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        GLint info_log_point_length;
        glGetShaderiv(result, GL_INFO_LOG_LENGTH, &info_log_point_length);
        std::string info_log(info_log_point_length, '\0');
        glGetShaderInfoLog(result, info_log.size(), nullptr, info_log.data());
        throw std::runtime_error("Shader compilation failed: " + info_log);
    }
    return result;
}

ShaderProgram::ShaderProgram(std::string vertex_shader_path, std::string fragment_shader_path) {
    std::string vertex_shader_source = read_shader(vertex_shader_path);
    std::string fragment_shader_source = read_shader(fragment_shader_path);

    GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source.c_str());
    GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source.c_str());

    id = glCreateProgram();
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);

    GLint status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        GLint info_log_point_length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_point_length);
        std::string info_log(info_log_point_length, '\0');
        glGetProgramInfoLog(id, info_log.size(), nullptr, info_log.data());
        throw std::runtime_error("Program linkage failed: " + info_log);
    }

    glGenVertexArrays(1, &vao);
}

ShaderProgram::Texture::Texture(
    const ShaderProgram* program,
    const GLchar* name,
    GLenum target,
    unsigned int texture_num
):
    target(target), texture_num(texture_num), program(program)
{
    glUseProgram(program->id);
    texture_location = glGetUniformLocation(program->id, name);
    glUniform1i(texture_location, texture_num);
}

void ShaderProgram::Texture::bind(GLuint texture_source)
{
    glUseProgram(program->id);
    glActiveTexture(GL_TEXTURE0 + texture_num);
    glBindTexture(target, texture_source);
}