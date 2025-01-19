#pragma once
#include "programs/shader_program.h"
#include "programs/external_structs/pool_coordinates/pool_coordinates.h"
#include <libs/glm/vec3.hpp>
#include <libs/glm/mat4x4.hpp>
#include <vector>

class PoolProgram : public ShaderProgram {
public:
    PoolProgram(std::string vertex_shader_path, std::string fragment_shader_path,
               PoolCoordinates coordinates,
               GLuint bottom_texture_source,
               std::vector<GLuint> caustic_texture_sources);

    void set_model(glm::mat4 model);
    void set_view(glm::mat4 model);
    void set_projection(glm::mat4 model);
    void set_sun_direction(glm::vec3 sun_direction);
    void set_wall_texture(GLuint new_wall_texture_source);

    void run() override;
private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    Texture albedo_texture;
    Texture caustics_texture;

    std::vector<GLuint> caustic_texture_sources;

    GLuint bottom_texture_source;
    GLuint wall_texture_source;

    PoolCoordinates coordinates;

    GLuint model_location;
    GLuint view_location;
    GLuint projection_location;
    GLuint sun_direction_location;
};