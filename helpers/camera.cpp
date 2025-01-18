#include "camera.h"

Camera::Camera(glm::vec3 position, float elevation, float azimuth)
{
    _position = position;
    _elevation = elevation;
    _azimuth = azimuth;
    _speed = NORMAL_SPEED;
}

void Camera::rotate_down(float angle)
{
    _elevation -= angle;
}

void Camera::rotate_up(float angle)
{
    _elevation += angle;
}

void Camera::rotate_left(float angle)
{
    _azimuth -= angle;
}

void Camera::rotate_right(float angle)
{
    _azimuth += angle;
}

void Camera::set_big_speed()
{
    _speed = BIG_SPEED;
}

void Camera::set_normal_speed()
{
    _speed = NORMAL_SPEED;
}

void Camera::go(glm::vec3 direction, float dt)
{
    glm::mat4 rotation_inv = glm::rotate(glm::rotate(glm::identity<glm::mat4>(), -_azimuth, {0.f, 1.f, 0.f}), -_elevation, {1.f, 0.f, 0.f});
    _position += glm::normalize((rotation_inv * glm::vec4(direction, 1.0f)).xyz()) * dt * _speed;
}

glm::mat4 Camera::get_view()
{
    glm::mat4 rotation = glm::rotate(glm::rotate(glm::identity<glm::mat4>(), _elevation, {1.f, 0.f, 0.f}), _azimuth, {0.f, 1.f, 0.f});
    return glm::translate(rotation, -_position);
}

glm::vec3 Camera::get_position()
{
    return _position;
}

std::ostream& operator<<(std::ostream& out, const Camera &camera) {
    out << "asimuth: " << camera._azimuth << std::endl;
    out << "elevation: " << camera._elevation << std::endl;
    return out << "position: " << camera._position.x << ' ' << camera._position.y << ' ' << camera._position.z;
}