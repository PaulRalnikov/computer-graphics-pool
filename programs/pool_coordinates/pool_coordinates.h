#pragma once
#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <libs/glm/vec3.hpp>

struct Rectangle{
    glm::vec3 corner;
    glm::vec3 x_side;
    glm::vec3 y_side;

    Rectangle(glm::vec3 corner, glm::vec3 x_side, glm::vec3 y_side);

    glm::vec3 operator[] (size_t i);
    glm::vec3 normal() const;
};

class PoolCoordinates
{
public:
    float width;
    float length;
    float height;
    glm::vec3 bottom_corner;

    PoolCoordinates(float width, float length, float height, glm::vec3 bottom_center);

    glm::vec3 x() const;
    glm::vec3 y() const;
    glm::vec3 z() const;

    Rectangle get_bottom() const;
    Rectangle get_front() const;
    Rectangle get_right() const;
    Rectangle get_back() const;
    Rectangle get_left() const;
};