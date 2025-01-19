#include "water_surface.h"
#include <vector>
#include <cmath>
#include <libs/glm/vec2.hpp>
#include <libs/glm/vec3.hpp>

WaterSurface::WaterSurface(const PoolCoordinates& coordinates, size_t quality):
    coordinates(coordinates), quality(quality)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)(0));
}

float f(glm::vec2 coords, float time) {
    return (std::sin(coords.x / 2.0 + time) + cos(coords.y / 2.0 + time)) / 2.0;
}

void WaterSurface::fetch_time(float time) {
    glm::vec3 left_vertex = coordinates.bottom_corner + glm::vec3(0.0, coordinates.height, 0.0);
    std::vector<glm::vec3> vertexes;
    for (int i = 0; i < quality; i++)
    {
        for (int j = 0; j < quality; j++)
        {
            glm::vec2 water_coords(coordinates.length * i / (quality - 1), coordinates.width * j / (quality - 1));
            glm::vec3 position = left_vertex + glm::vec3(water_coords.x, f(water_coords, time), water_coords.y);
            vertexes.push_back(position);
        }
    }

    std::vector<uint32_t> indexes;
    for (int i = 0; i < quality - 1; i++)
    {
        for (int j = 0; j < quality - 1; j++)
        {
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

void WaterSurface::bind_buffers() const {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

size_t WaterSurface::get_indexes_count() const {
    return (quality - 1) * (quality - 1) * 6;
}

const PoolCoordinates& WaterSurface::get_pool_coordinates() const
{
    return coordinates;
}