#include "water.h"
#include <vector>
#include <iostream>
#include <libs/glm/gtx/string_cast.hpp>

WaterProgram::WaterProgram(std::string vertex_shader_path, std::string fragment_shader_path, WaterSurface& surface):
    ShaderProgram(vertex_shader_path, fragment_shader_path),
    surface(surface),
    bottom_texture(this, "bottom_texture", GL_TEXTURE_2D, 0),
    wall_texture(this, "wall_texture", GL_TEXTURE_2D, 1),
    environment_texture(this, "environment_texture", GL_TEXTURE_2D, 2)
{
    model_location = glGetUniformLocation(id, "model");
    view_location = glGetUniformLocation(id, "view");
    projection_location = glGetUniformLocation(id, "projection");
    camera_position_location = glGetUniformLocation(id, "camera_position");
    sun_direction_location = glGetUniformLocation(id, "sun_direction");
    time_location = glGetUniformLocation(id, "time");

    glUseProgram(id);
    const PoolCoordinates& coordinates = surface.get_pool_coordinates();
    Rectangle bottom = coordinates.get_bottom();
    GLuint bottom_angle_location = glGetUniformLocation(id, "bottom_angle");
    GLuint bottom_x_side_location = glGetUniformLocation(id, "bottom_x_side");
    GLuint bottom_y_side_location = glGetUniformLocation(id, "bottom_y_side");
    glUniform3fv(bottom_angle_location, 1, reinterpret_cast<float *>(&bottom.corner));
    glUniform3fv(bottom_x_side_location, 1, reinterpret_cast<float *>(&bottom.x_side));
    glUniform3fv(bottom_y_side_location, 1, reinterpret_cast<float *>(&bottom.y_side));

    Rectangle front = coordinates.get_front();
    GLuint front_angle_location = glGetUniformLocation(id, "front_angle");
    GLuint front_x_side_location = glGetUniformLocation(id, "front_x_side");
    GLuint front_y_side_location = glGetUniformLocation(id, "front_y_side");
    glUniform3fv(front_angle_location, 1, reinterpret_cast<float *>(&front.corner));
    glUniform3fv(front_x_side_location, 1, reinterpret_cast<float *>(&front.x_side));
    glUniform3fv(front_y_side_location, 1, reinterpret_cast<float *>(&front.y_side));

    Rectangle right = coordinates.get_right();
    GLuint right_angle_location = glGetUniformLocation(id, "right_angle");
    GLuint right_x_side_location = glGetUniformLocation(id, "right_x_side");
    GLuint right_y_side_location = glGetUniformLocation(id, "right_y_side");
    glUniform3fv(right_angle_location, 1, reinterpret_cast<float *>(&right.corner));
    glUniform3fv(right_x_side_location, 1, reinterpret_cast<float *>(&right.x_side));
    glUniform3fv(right_y_side_location, 1, reinterpret_cast<float *>(&right.y_side));

    Rectangle back = coordinates.get_back();
    GLuint back_angle_location = glGetUniformLocation(id, "back_angle");
    GLuint back_x_side_location = glGetUniformLocation(id, "back_x_side");
    GLuint back_y_side_location = glGetUniformLocation(id, "back_y_side");
    glUniform3fv(back_angle_location, 1, reinterpret_cast<float *>(&back.corner));
    glUniform3fv(back_x_side_location, 1, reinterpret_cast<float *>(&back.x_side));
    glUniform3fv(back_y_side_location, 1, reinterpret_cast<float *>(&back.y_side));

    Rectangle left = coordinates.get_left();
    GLuint left_angle_location = glGetUniformLocation(id, "left_angle");
    GLuint left_x_side_location = glGetUniformLocation(id, "left_x_side");
    GLuint left_y_side_location = glGetUniformLocation(id, "left_y_side");
    glUniform3fv(left_angle_location, 1, reinterpret_cast<float *>(&left.corner));
    glUniform3fv(left_x_side_location, 1, reinterpret_cast<float *>(&left.x_side));
    glUniform3fv(left_y_side_location, 1, reinterpret_cast<float *>(&left.y_side));
}

void WaterProgram::set_model(glm::mat4 model)
{
    glUseProgram(id);
    glUniformMatrix4fv(model_location, 1, GL_FALSE, reinterpret_cast<float *>(&model));
}

void WaterProgram::set_view(glm::mat4 view)
{
    glUseProgram(id);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, reinterpret_cast<float *>(&view));
}

void WaterProgram::set_projection(glm::mat4 projection)
{
    glUseProgram(id);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, reinterpret_cast<float *>(&projection));
}

void WaterProgram::set_camera_position(glm::vec3 camera_position)
{
    glUseProgram(id);
    glUniform3fv(camera_position_location, 1, reinterpret_cast<float *>(&camera_position));
}

void WaterProgram::set_sun_direction(glm::vec3 sun_direction)
{
    glUseProgram(id);
    glUniform3fv(sun_direction_location, 1, reinterpret_cast<float *>(&sun_direction));
}

void WaterProgram::set_bottom_texture(GLuint bottom_texture_source)
{
    bottom_texture.bind(bottom_texture_source);
}

void WaterProgram::set_wall_texture(GLuint wall_texture_source)
{
    wall_texture.bind(wall_texture_source);
}

void WaterProgram::set_environment_texture(GLuint environment_texture_source) {
    environment_texture.bind(environment_texture_source);
}

void WaterProgram::set_time(float time) {
    glUseProgram(id);
    glUniform1f(time_location, time);
}

void WaterProgram::run() {
    glUseProgram(id);
    surface.bind_buffers();

    glDrawElements(GL_TRIANGLES, surface.get_indexes_count(), GL_UNSIGNED_INT, (void *)(0));
}