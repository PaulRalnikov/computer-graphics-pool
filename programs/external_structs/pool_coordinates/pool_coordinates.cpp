#include "pool_coordinates.h"
#include <libs/glm/mat4x4.hpp>
#include <stdexcept>
#include <string>

Rectangle::Rectangle(glm::vec3 corner, glm::vec3 x_side, glm::vec3 y_side):
    corner(corner), x_side(x_side), y_side(y_side) {}

glm::vec3 Rectangle::normal() const {
    return glm::normalize(glm::cross(x_side, y_side));
}

glm::vec3 Rectangle::operator[](size_t i) {
    if (i == 0) return corner;
    if (i == 1) return corner + x_side;
    if (i == 2) return corner + x_side + y_side;
    if (i == 3) return corner + y_side;
    throw std::runtime_error("Too large index: expected <= 3, but given " + std::to_string(i));
}

PoolCoordinates::PoolCoordinates(float width, float length, float height, glm::vec3 bottom_corner) :
    width(width), length(length), height(height), bottom_corner(bottom_corner) {}

glm::vec3 PoolCoordinates::x() const {
    return glm::vec3(length, 0.0, 0.0);
}

glm::vec3 PoolCoordinates::y() const{
    return glm::vec3(0.0, height, 0.0);
}

glm::vec3 PoolCoordinates::z() const {
    return glm::vec3(0.0, 0.0, width);
}

Rectangle PoolCoordinates::get_bottom() const{
    return Rectangle(bottom_corner, z(), x());
}

Rectangle PoolCoordinates::get_front() const
{
    return Rectangle(bottom_corner, x(), y());
}

Rectangle PoolCoordinates::get_right() const
{
    return Rectangle(bottom_corner + x(), z(), y());
}

Rectangle PoolCoordinates::get_back() const
{
    return Rectangle(bottom_corner + x() + z(), -x(), y());
}

Rectangle PoolCoordinates::get_left() const
{
    return Rectangle(bottom_corner + z(), -z(), y());
}

Rectangle PoolCoordinates::get_side(size_t side_index) const
{
    if (side_index == 0) return get_bottom();
    if (side_index == 1) return get_front();
    if (side_index == 2) return get_right();
    if (side_index == 3) return get_back();
    if (side_index == 4) return get_left();
    throw std::runtime_error("Too large side index");
}