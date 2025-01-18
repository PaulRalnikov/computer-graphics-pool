#include "pool.h"
#include <libs/glm/vec2.hpp>
#include <vector>
#include <iostream>

struct PoolVertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoords;

    PoolVertex(glm::vec3 position = glm::vec3(0.0), glm::vec3 normal = glm::vec3(0.0), glm::vec2 texcoords = glm::vec2(0.0)):
        position(position), normal(normal), texcoords(texcoords) {}
};

PoolProgram::PoolProgram(std::string vertex_shader_path, std::string fragment_shader_path,
                          PoolCoordinates coordinates, GLuint bottom_texture, GLuint wall_texture):
    ShaderProgram(vertex_shader_path, fragment_shader_path),
    coordinates(coordinates),
    bottom_texture(bottom_texture),
    wall_texture(wall_texture),
    texture(this, "albedo_texture", GL_TEXTURE_2D, 0)
{

    model_location = glGetUniformLocation(id, "model");
    view_location = glGetUniformLocation(id, "view");
    projection_location = glGetUniformLocation(id, "projection");
    sun_direction_location = glGetUniformLocation(id, "sun_direction");

    glBindVertexArray(vao);

    glm::vec3 left_vector(-coordinates.length / 2, 0.0, -coordinates.width / 2);
    glm::vec3 right_vector(coordinates.length / 2, 0.0, -coordinates.width / 2);

    glm::vec3 bottom_1 = coordinates.bottom_center + left_vector;
    glm::vec3 bottom_2 = coordinates.bottom_center + right_vector;
    glm::vec3 bottom_3 = coordinates.bottom_center - left_vector;
    glm::vec3 bottom_4 = coordinates.bottom_center - right_vector;

    glm::vec3 to_top_vec(0.0, coordinates.height, 0.0);

    glm::vec3 top_1 = bottom_1 + to_top_vec;
    glm::vec3 top_2 = bottom_2 + to_top_vec;
    glm::vec3 top_3 = bottom_3 + to_top_vec;
    glm::vec3 top_4 = bottom_4 + to_top_vec;

    glm::vec3 back_normal(-1, 0, 0);
    glm::vec3 botoom_normal(0, 1, 0);
    glm::vec3 front_normal(1, 0, 0);
    glm::vec3 right_normal(0, 0, -1);
    glm::vec3 left_normal(0, 0, 1);

    std::vector<PoolVertex> vertexes = {
        // bottom
        PoolVertex(bottom_1, botoom_normal, glm::vec2(0.0, 1.0)),
        PoolVertex(bottom_2, botoom_normal, glm::vec2(1.0, 1.0)),
        PoolVertex(bottom_3, botoom_normal, glm::vec2(1.0, 0.0)),
        PoolVertex(bottom_4, botoom_normal, glm::vec2(0.0, 0.0)),
        // front
        PoolVertex(bottom_2, front_normal, glm::vec2(0.0, 1.0)),
        PoolVertex(bottom_1, front_normal, glm::vec2(1.0, 1.0)),
        PoolVertex(top_1, front_normal, glm::vec2(1.0, 0.0)),
        PoolVertex(top_2, front_normal, glm::vec2(0.0, 0.0)),
        // right
        PoolVertex(bottom_3, right_normal, glm::vec2(0.0, 1.0)),
        PoolVertex(bottom_2, right_normal, glm::vec2(1.0, 1.0)),
        PoolVertex(top_2, right_normal, glm::vec2(1.0, 0.0)),
        PoolVertex(top_3, right_normal, glm::vec2(0.0, 0.0)),
        // back
        PoolVertex(bottom_4, back_normal, glm::vec2(0.0, 1.0)),
        PoolVertex(bottom_3, back_normal, glm::vec2(1.0, 1.0)),
        PoolVertex(top_3, back_normal, glm::vec2(1.0, 0.0)),
        PoolVertex(top_4, back_normal, glm::vec2(0.0, 0.0)),
        // left
        PoolVertex(bottom_1, left_normal, glm::vec2(0.0, 1.0)),
        PoolVertex(bottom_4, left_normal, glm::vec2(1.0, 1.0)),
        PoolVertex(top_4, left_normal, glm::vec2(1.0, 0.0)),
        PoolVertex(top_1, left_normal, glm::vec2(0.0, 0.0)),
    };

    std::vector<uint32_t> indices;
    for (int i = 0; i < 5; i++) {
        indices.push_back(i * 4 + 0);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 1);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 0);
        indices.push_back(i * 4 + 3);
    }

    bottom_vertex_segment_start = 0;
    bottom_vertex_segment_length = 6;

    wall_vertex_segment_start = 6;
    wall_vertex_segment_length = 24;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(vertexes[0]), vertexes.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PoolVertex), (void *)offsetof(PoolVertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PoolVertex), (void *)offsetof(PoolVertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PoolVertex), (void *)offsetof(PoolVertex, texcoords));
}

void PoolProgram::set_model(glm::mat4 model) {
    glUseProgram(id);
    glUniformMatrix4fv(model_location, 1, GL_FALSE, reinterpret_cast<float *>(&model));
}

void PoolProgram::set_view(glm::mat4 view)
{
    glUseProgram(id);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, reinterpret_cast<float *>(&view));
}

void PoolProgram::set_projection(glm::mat4 projection)
{
    glUseProgram(id);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, reinterpret_cast<float *>(&projection));
}

void PoolProgram::set_sun_direction(glm::vec3 sun_direction) {
    glUseProgram(id);
    glUniform3fv(sun_direction_location, 1, reinterpret_cast<float *>(&sun_direction));
}

void PoolProgram::run() {
    glUseProgram(id);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    texture.bind(this, bottom_texture);
    glDrawElements(GL_TRIANGLES, bottom_vertex_segment_length, GL_UNSIGNED_INT, (void *)(bottom_vertex_segment_start * sizeof(int)));
    texture.bind(this, wall_texture);
    glDrawElements(GL_TRIANGLES, wall_vertex_segment_length, GL_UNSIGNED_INT, (void *)(wall_vertex_segment_start * sizeof(int)));
}