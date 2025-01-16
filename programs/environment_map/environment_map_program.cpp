#include "environment_map_program.h"

EnvironmentMapProgram::EnvironmentMapProgram(std::string vertex_shader_path, std::string fragment_shader_path):
    ShaderProgram(vertex_shader_path, fragment_shader_path),
    environment_texture(this, "environment_texture", GL_TEXTURE_2D, 0)
{
    view_inverse_location = glGetUniformLocation(id, "view_projection_inverse");
    camera_position_location = glGetUniformLocation(id, "camera_position");
}

void EnvironmentMapProgram::set_view_inverse(glm::mat4 view_inverse) {
    glUseProgram(id);
    glUniformMatrix4fv(view_inverse_location, 1, GL_FALSE, reinterpret_cast<float *>(&view_inverse));
}

void EnvironmentMapProgram::set_camera_position(glm::vec3 camera_position) {
    glUseProgram(id);
    glUniform3fv(camera_position_location, 1, reinterpret_cast<float *>(&camera_position));
}

void EnvironmentMapProgram::set_environment_texture(GLuint environment_texture_source) {
    glUseProgram(id);
    environment_texture.bind(this, environment_texture_source);
}

void EnvironmentMapProgram::run() {
    glUseProgram(id);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}