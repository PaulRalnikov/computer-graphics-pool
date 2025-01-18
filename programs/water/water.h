#pragma once
#include "programs/shader_program.h"
#include "programs/pool_coordinates/pool_coordinates.h"
#include <libs/glm/vec3.hpp>
#include <libs/glm/mat4x4.hpp>

class WaterProgram : public ShaderProgram {
public:
    WaterProgram(std::string vertex_shader_path, std::string fragment_shader_path,
                 PoolCoordinates coordinates, size_t quality = 128);

    void set_model(glm::mat4 model);
    void set_view(glm::mat4 model);
    void set_projection(glm::mat4 model);
    void set_camera_position(glm::vec3 camera_position);
    void set_bottom_texture(GLuint bottom_texture_source);
    void set_environment_texture(GLuint environment_texture_source);
    void fetch_time(float time);

    void run() override;

private:
    PoolCoordinates coordinates;
    size_t quality;

    GLuint vbo;
    GLuint ebo;

    GLuint model_location;
    GLuint view_location;
    GLuint projection_location;
    GLuint camera_position_location;
    GLuint time_location;

    GLuint bottom_angle_location;
    GLuint bottom_normal_location;
    GLuint bottom_size_location;
    Texture bottom_texture;

    Texture environment_texture;
};