#pragma once
#include "programs/shader_program.h"
#include "programs/external_structs/water_surface/water_surface.h"
#include <libs/glm/vec3.hpp>
#include <libs/glm/mat4x4.hpp>

class CausticsProgram : public ShaderProgram {
public:
    CausticsProgram(
        std::string vertex_shader_path,
        std::string fragment_shader_path,
        const WaterSurface& surface,
        unsigned int caustic_texture_resolution
    );

    void set_model(glm::mat4 model);
    void set_corner(glm::vec3 corner);
    void set_x_side_vector(glm::vec3 x_side_vector);
    void set_y_side_vector(glm::vec3 y_side_vector);
    void set_sun_direction(glm::vec3 sun_direction);
    void set_time(float time);
    GLuint get_caustics_texture_source() const;
    void set_caustics_texture_source(GLuint new_caustics_texture_source);

    GLuint get_fbo();

    void run() override;
private:
    const WaterSurface& surface;
    GLuint fbo;

    GLuint caustics_texture_source;
    unsigned int caustics_texture_resolution;  

    GLuint model_location;
    GLuint corner_location;
    GLuint x_side_vector_location;
    GLuint y_side_vector_location;
    GLuint sun_direction_location;
    GLuint time_location;
};