#pragma once
#include "programs/shader_program.h"
#include "programs/external_structs/pool_coordinates/pool_coordinates.h"
#include "programs/external_structs/water_surface/water_surface.h"
#include <libs/glm/vec3.hpp>
#include <libs/glm/mat4x4.hpp>

class WaterProgram : public ShaderProgram {
public:
    WaterProgram(std::string vertex_shader_path, std::string fragment_shader_path, WaterSurface& surface);

    void set_model(glm::mat4 model);
    void set_view(glm::mat4 model);
    void set_projection(glm::mat4 model);
    void set_camera_position(glm::vec3 camera_position);
    void set_sun_direction(glm::vec3 sun_direction);
    void set_bottom_texture(GLuint bottom_texture_source);
    void set_wall_texture(GLuint wall_texture_source);
    void set_environment_texture(GLuint environment_texture_source);
    void set_time(float time);

    void run() override;

private:
    WaterSurface& surface;
    GLuint model_location;
    GLuint view_location;
    GLuint projection_location;
    GLuint camera_position_location;
    GLuint sun_direction_location;
    GLuint time_location;
    
    Texture bottom_texture;
    Texture wall_texture;
    Texture environment_texture;
};