#pragma once
#include "programs/shader_program.h"
#include "programs/external_structs/water_surface/water_surface.h"

class CausticsProgram : public ShaderProgram {
public:
    CausticsProgram(
        std::string vertex_shader_path,
        std::string fragment_shader_path,
        const WaterSurface& surface,
        unsigned int caustic_texture_resolution
    );

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