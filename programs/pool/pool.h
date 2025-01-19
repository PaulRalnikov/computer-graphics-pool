#pragma once
#include "programs/shader_program.h"
#include "programs/external_structs/pool_coordinates/pool_coordinates.h"
#include <libs/glm/vec3.hpp>
#include <libs/glm/mat4x4.hpp>

class PoolProgram : public ShaderProgram {
public:
    PoolProgram(std::string vertex_shader_path, std::string fragment_shader_path,
                PoolCoordinates coordinates, GLuint bottom_texture_source); 

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
    Texture texture;

    GLuint bottom_texture_source;
    GLuint wall_texture_source;

    PoolCoordinates coordinates;

    GLuint model_location;
    GLuint view_location;
    GLuint projection_location;
    GLuint sun_direction_location;

    int bottom_vertex_segment_start;
    int bottom_vertex_segment_length;

    int wall_vertex_segment_start;
    int wall_vertex_segment_length;
};