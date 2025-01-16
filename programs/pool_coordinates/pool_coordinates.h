#pragma once
#include <libs/glm/vec3.hpp>

struct PoolCoordinates
{
    float width;
    float length;
    float height;
    glm::vec3 bottom_center;

    PoolCoordinates(float width, float length, float height, glm::vec3 bottom_center);
};