#pragma once
#include "../shader_program.h"
#include <libs/glm/vec3.hpp>
#include <libs/glm/mat4x4.hpp>

class EnvironmentMapProgram : public ShaderProgram {
public:
    EnvironmentMapProgram(std::string vertex_shader_path, std::string fragment_shader_path);

    void set_view_inverse(glm::mat4 projection_inverse);
    void set_camera_position(glm::vec3 camera_position);
    void set_environment_texture(GLuint environment_texture);

    void run() override;
private:
    GLuint view_inverse_location;
    GLuint camera_position_location;
    Texture environment_texture;
};