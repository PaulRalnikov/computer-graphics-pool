#pragma once

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <libs/glm/vec3.hpp>
#include <libs/glm/vec4.hpp>
#include <libs/glm/mat4x4.hpp>
#include <libs/glm/ext/matrix_transform.hpp>
#include <libs/glm/ext/matrix_clip_space.hpp>
#include <libs/glm/ext/scalar_constants.hpp>
#include <libs/glm/gtx/string_cast.hpp>
#include <iostream>

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0),
           float elevation = 45.f,
           float azimuth = 0.f);

    void rotate_down(float angle);
    void rotate_up(float angle);
    void rotate_left(float angle);
    void rotate_right(float angle);

    void set_big_speed();

    void set_normal_speed();

    void go(glm::vec3 direction, float dt);

    glm::mat4 get_view();

    glm::vec3 get_position();

    friend std::ostream& operator<<(std::ostream& out, const Camera& camera);

private:
    const static inline float NORMAL_SPEED = 10.f;
    const static inline float BIG_SPEED = 20.f;

    glm::vec3 _position;
    float _elevation;
    float _azimuth;
    float _speed;
};
