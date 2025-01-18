#include "water.h"
#include <vector>
#include <iostream>
#include <libs/glm/gtx/string_cast.hpp>

struct WaterVertex{
    glm::vec3 position;
    glm::vec3 normal;

    WaterVertex(glm::vec3 position = glm::vec3(0.0), glm::vec3 normal = glm::vec3(0.0)) :
        position(position), normal(normal){}
};

WaterProgram::WaterProgram(std::string vertex_shader_path, std::string fragment_shader_path,
                           PoolCoordinates coordinates, size_t quality):
    ShaderProgram(vertex_shader_path, fragment_shader_path),
    coordinates(coordinates),
    quality(quality),
    bottom_texture(this, "bottom_texture", GL_TEXTURE_2D, 1),
    environment_texture(this, "environment_texture", GL_TEXTURE_2D, 0)
{
    model_location = glGetUniformLocation(id, "model");
    view_location = glGetUniformLocation(id, "view");
    projection_location = glGetUniformLocation(id, "projection");
    camera_position_location = glGetUniformLocation(id, "camera_position");

    bottom_angle_location = glGetUniformLocation(id, "bottom_angle");
    bottom_normal_location = glGetUniformLocation(id, "bottom_normal");
    bottom_size_location = glGetUniformLocation(id, "bottom_size");

    glUseProgram(id);
    glUniform3fv(bottom_angle_location, 1, reinterpret_cast<float *>(&coordinates.bottom_angle));
    glUniform3f(bottom_normal_location, 0.0, 1.0, 0.0);
    glUniform2f(bottom_size_location, coordinates.length, coordinates.width);

    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), (void *)offsetof(WaterVertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), (void *)offsetof(WaterVertex, normal));
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

void WaterProgram::set_bottom_texture(GLuint bottom_texture_source)
{
    bottom_texture.bind(this, bottom_texture_source);
}

void WaterProgram::set_environment_texture(GLuint environment_texture_source) {
    environment_texture.bind(this, environment_texture_source);
}

float f(glm::vec2 coords, float time) {
    return std::sin(coords.x + time) + cos(coords.y + time);
}

float dfdx(glm::vec2 coords, float time) {
    return std::cos(coords.x + time);
}

float dfdy(glm::vec2 coords, float time) {
    return -std::sin(coords.y + time);
}

void WaterProgram::fetch_time(float time) {
    glm::vec3 left_vertex = coordinates.bottom_angle + glm::vec3(0.0, coordinates.height, 0.0);
    std::vector<WaterVertex> vertexes;
    for (int i = 0; i < quality; i++) {
        for (int j = 0; j < quality; j++) {
            glm::vec2 water_coords(coordinates.length * i / (quality - 1), coordinates.width * j / (quality - 1));
            glm::vec3 position = left_vertex + glm::vec3(water_coords.x, f(water_coords, time), water_coords.y);
            glm::vec3 normal = glm::normalize(glm::vec3(-dfdx(water_coords, time), 1.0, -dfdy(water_coords, time)));
            vertexes.push_back(WaterVertex(position, normal));
        }
    }

    std::vector<uint32_t> indexes;
    for (int i = 0; i < quality - 1; i++) {
        for (int j = 0; j < quality - 1; j++) {
            indexes.push_back((i + 0) * quality + j + 0);
            indexes.push_back((i + 0) * quality + j + 1);
            indexes.push_back((i + 1) * quality + j + 1);

            indexes.push_back((i + 1) * quality + j + 1);
            indexes.push_back((i + 1) * quality + j + 0);
            indexes.push_back((i + 0) * quality + j + 0);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(vertexes[0]), vertexes.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(indexes[0]), indexes.data(), GL_STATIC_DRAW);
}

void WaterProgram::run() {
    glUseProgram(id);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glDrawElements(GL_TRIANGLES, (quality - 1) * (quality - 1) * 6, GL_UNSIGNED_INT, (void *)(0));
}