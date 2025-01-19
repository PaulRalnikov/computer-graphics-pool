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

std::vector<PoolVertex> to_vector(Rectangle rec) {
    glm::vec3 normal = rec.normal();
    std::vector<PoolVertex> result = {
        PoolVertex(rec[0], normal, glm::vec2(0.0, 0.0)),
        PoolVertex(rec[1], normal, glm::vec2(1.0, 0.0)),
        PoolVertex(rec[2], normal, glm::vec2(1.0, 1.0)),
        PoolVertex(rec[3], normal, glm::vec2(0.0, 1.0)),
    };
    return result;
}

PoolProgram::PoolProgram(std::string vertex_shader_path, std::string fragment_shader_path,
                          PoolCoordinates coordinates, GLuint bottom_texture_source):
    ShaderProgram(vertex_shader_path, fragment_shader_path),
    coordinates(coordinates),
    bottom_texture_source(bottom_texture_source),
    texture(this, "albedo_texture", GL_TEXTURE_2D, 0)
{

    model_location = glGetUniformLocation(id, "model");
    view_location = glGetUniformLocation(id, "view");
    projection_location = glGetUniformLocation(id, "projection");
    sun_direction_location = glGetUniformLocation(id, "sun_direction");

    glBindVertexArray(vao);

    std::vector<Rectangle> rectangles = {
        coordinates.get_bottom(),
        coordinates.get_right(),
        coordinates.get_back(),
        coordinates.get_left(),
        coordinates.get_front(),
    };

    std::vector<PoolVertex> vertexes;
    std::vector<uint32_t> indices;

    for (size_t i = 0; i < rectangles.size(); i++) {
        for (auto el : to_vector(rectangles[i]))
            vertexes.push_back(el);
        indices.push_back(i * 4 + 0);
        indices.push_back(i * 4 + 1);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 3);
        indices.push_back(i * 4 + 0);
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

void PoolProgram::set_wall_texture(GLuint new_wall_texture_source) {
    wall_texture_source = new_wall_texture_source;
}

    void PoolProgram::run()
{
    glUseProgram(id);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    texture.bind(bottom_texture_source);
    glDrawElements(GL_TRIANGLES, bottom_vertex_segment_length, GL_UNSIGNED_INT, (void *)(bottom_vertex_segment_start * sizeof(int)));
    texture.bind(wall_texture_source);
    glDrawElements(GL_TRIANGLES, wall_vertex_segment_length, GL_UNSIGNED_INT, (void *)(wall_vertex_segment_start * sizeof(int)));
}