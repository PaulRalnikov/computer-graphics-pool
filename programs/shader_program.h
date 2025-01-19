#pragma once
#include <GL/glew.h>
#include <string>

class ShaderProgram {
public:
    ShaderProgram(std::string vertex_shader_path, std::string fragment_shader_path);

    class Texture {
    public:
        Texture(const ShaderProgram *program, const GLchar* name, GLenum target, unsigned int texture_num);

        void bind(GLuint texture_source);

    private:
        const ShaderProgram* program; 
        GLuint texture_location;
        GLenum target;
        unsigned int texture_num;
    };

    virtual void run() = 0;
protected:
    GLuint id;
};