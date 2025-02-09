#include "blur.h"
#include <stdexcept>

BlurProgram::BlurProgram(std::string vertex_shader_source, std::string fragment_shader_source,
                         GLuint swap_texture_source, unsigned int textures_resolution, GLuint draw_fbo) :
        ShaderProgram(vertex_shader_source, fragment_shader_source),
        in_texture(this, "in_texture", GL_TEXTURE_2D, 0),
        swap_texture(this, "in_texture", GL_TEXTURE_2D, 1),
        swap_texture_source(swap_texture_source),
        textures_resolution(textures_resolution),
        draw_fbo(draw_fbo)
{
    glUseProgram(id);
    glGenVertexArrays(1, &vao);
    glGenFramebuffers(1, &swap_fbo);
    blur_dir_location = glGetUniformLocation(id, "dir");
}

void BlurProgram::attach_draw_texture_source(GLuint new_blur_texture_source)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, swap_fbo);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, new_blur_texture_source, 0);
    if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Incomplete framebuffer!");
}

void BlurProgram::set_blur_texture(GLuint blur_texture_source) {
    this->blur_texture_source = blur_texture_source;
}

void BlurProgram::run() {
    glUseProgram(id);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    GLint old_viewport[4];
    glGetIntegerv(GL_VIEWPORT, old_viewport);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindVertexArray(vao);
    //draw to swap texture
    attach_draw_texture_source(swap_texture_source);
    swap_texture.bind(blur_texture_source);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, textures_resolution, textures_resolution);
    glUniform2f(blur_dir_location, 0.f, 1.f / textures_resolution);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // draw to swap texture
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_fbo);
    glClear(GL_COLOR_BUFFER_BIT);
    in_texture.bind(swap_texture_source);
    glViewport(0, 0, textures_resolution, textures_resolution);
    glUniform2f(blur_dir_location, 1.f / textures_resolution, 0.f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
