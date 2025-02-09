#pragma once
#include "programs/shader_program.h"
#include <string>

class BlurProgram : public ShaderProgram {
public:
    BlurProgram(std::string vertex_shader_source, std::string fragment_shader_source,
                GLuint swap_texture_source, unsigned int textures_resolution, GLuint draw_fbo);

    void set_blur_texture(GLuint blur_texture_source);

    void run() override;
private:
    void attach_draw_texture_source(GLuint new_blur_texture_source);

    GLuint vao;
    GLuint swap_fbo;
    GLuint draw_fbo;

    GLuint blur_texture_source;
    GLuint swap_texture_source;
    unsigned int textures_resolution;

    Texture in_texture;
    Texture swap_texture;
    GLuint blur_dir_location;
};