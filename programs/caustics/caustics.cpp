#include "caustics.h"
#include <stdexcept>

CausticsProgram::CausticsProgram(
    std::string vertex_shader_path,
    std::string fragment_shader_path,
    const WaterSurface& surface,
    unsigned int caustics_texture_resolution
):
    ShaderProgram(vertex_shader_path, fragment_shader_path),
    surface(surface),
    caustics_texture_resolution(caustics_texture_resolution)
{
    model_location = glGetUniformLocation(id, "model");
    corner_location = glGetUniformLocation(id, "corner");
    x_side_vector_location = glGetUniformLocation(id, "x_side_vector");
    y_side_vector_location = glGetUniformLocation(id, "y_side_vector");
    sun_direction_location = glGetUniformLocation(id, "sun_direction");
    time_location = glGetUniformLocation(id, "time");
    
    glUseProgram(id);
    glGenTextures(1, &caustics_texture_source);
    glBindTexture(GL_TEXTURE_2D, caustics_texture_source);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, caustics_texture_resolution, caustics_texture_resolution, 0, GL_RGBA, GL_FLOAT, nullptr);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, caustics_texture_source, 0);
    if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Incomplete framebuffer!");
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void CausticsProgram::set_model(glm::mat4 model) {
    glUseProgram(id);
    
}
void CausticsProgram::set_corner(glm::vec3 corner) {
    glUseProgram(id);
    glUniform3fv(corner_location, 1, reinterpret_cast<float *>(&corner));
}

void CausticsProgram::set_x_side_vector(glm::vec3 x_side_vector) {
    glUseProgram(id);
    glUniform3fv(x_side_vector_location, 1, reinterpret_cast<float *>(&x_side_vector));
}

void CausticsProgram::set_y_side_vector(glm::vec3 y_side_vector) {
    glUseProgram(id);
    glUniform3fv(y_side_vector_location, 1, reinterpret_cast<float *>(&y_side_vector));
}

void CausticsProgram::set_sun_direction(glm::vec3 sun_direction) {
    glUseProgram(id);
    glUniform3fv(sun_direction_location, 1, reinterpret_cast<float *>(&sun_direction));
}
void CausticsProgram::set_time(float time) {
    glUseProgram(id);
    glUniform1f(time_location, time);
}

void CausticsProgram::run() {
    glDisable(GL_CULL_FACE);
    glUseProgram(id);
    surface.bind_buffers();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glDrawElements(GL_TRIANGLES, surface.get_indexes_count(), GL_UNSIGNED_INT, (void *)(0));
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}