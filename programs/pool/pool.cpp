#include "pool.h"
#include <libs/glm/vec2.hpp>
#include <vector>
#include <array>
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
    return std::vector<PoolVertex> ({
        PoolVertex(rec[0], normal, glm::vec2(0.0, 0.0)),
        PoolVertex(rec[1], normal, glm::vec2(1.0, 0.0)),
        PoolVertex(rec[2], normal, glm::vec2(1.0, 1.0)),
        PoolVertex(rec[3], normal, glm::vec2(0.0, 1.0)),
    });
}

PoolProgram::PoolProgram(std::string vertex_shader_path, std::string fragment_shader_path,
                         PoolCoordinates coordinates,
                         GLuint bottom_texture_source,
                         std::vector<GLuint> caustic_texture_sources
):
    ShaderProgram(vertex_shader_path, fragment_shader_path),
    coordinates(coordinates),
    bottom_texture_source(bottom_texture_source),
    albedo_texture(this, "albedo_texture", GL_TEXTURE_2D, 0),
    caustics_texture(this, "caustics_texture", GL_TEXTURE_2D, 1),
    caustic_texture_sources(caustic_texture_sources)
{
    model_location = glGetUniformLocation(id, "model");
    view_location = glGetUniformLocation(id, "view");
    projection_location = glGetUniformLocation(id, "projection");
    sun_direction_location = glGetUniformLocation(id, "sun_direction");

    std::vector<Rectangle> rectangles;
    for (size_t i = 0; i < PoolCoordinates::NUM_SIDES; i++) {
        rectangles.push_back(coordinates.get_side(i));
    }

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

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

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

    for (size_t i = 0; i < caustic_texture_sources.size(); i++) {
        caustics_texture.bind(caustic_texture_sources[i]);
        if (i == 0)
            albedo_texture.bind(bottom_texture_source);
        else
            albedo_texture.bind(wall_texture_source);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(i * 6 * sizeof(int)));
    }
}