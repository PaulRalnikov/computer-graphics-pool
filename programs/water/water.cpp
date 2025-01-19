#include "water.h"
#include <array>
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

    GLuint corner_location = glGetUniformLocation(id, "corner");
    GLuint x_side_vector_location = glGetUniformLocation(id, "x_side_vector");
    GLuint y_side_vector_location = glGetUniformLocation(id, "y_side_vector");

    std::array<const GLchar*, NUM_SIDES> caustic_texture_names = {
        "bottom_caustics_texture",
        "front_caustics_texture",
        "right_caustics_texture",
        "back_caustics_texture",
        "left_caustics_texture"
    };

    for (int i = 0; i < NUM_SIDES; i++) {
        caustic_texture[i] = Texture(this, caustic_texture_names[i], GL_TEXTURE_2D, i + 3);
    }
    
    const PoolCoordinates &coordinates = surface.get_pool_coordinates();
    Rectangle bottom = coordinates.get_bottom();
    Rectangle front = coordinates.get_front();
    Rectangle right = coordinates.get_right();
    Rectangle back = coordinates.get_back();
    Rectangle left = coordinates.get_left();

    std::vector<glm::vec3> corners = {
        bottom.corner,
        front.corner,
        right.corner,
        back.corner,
        left.corner
    };
    glUniform3fv(corner_location, corners.size(), reinterpret_cast<float *>(corners.data()));

    std::vector<glm::vec3> x_side_vectors = {
        bottom.x_side,
        front.x_side,
        right.x_side,
        back.x_side,
        left.x_side
    };
    glUniform3fv(x_side_vector_location, x_side_vectors.size(), reinterpret_cast<float *>(x_side_vectors.data()));

    std::vector<glm::vec3> y_side_vectors = {
        bottom.y_side,
        front.y_side,
        right.y_side,
        back.y_side,
        left.y_side
    };
    glUniform3fv(y_side_vector_location, y_side_vectors.size(), reinterpret_cast<float *>(y_side_vectors.data()));
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

void WaterProgram::set_caustics_texture(size_t side_index, GLuint caustics_texture_source)
{
    caustic_texture[side_index].bind(caustics_texture_source);
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