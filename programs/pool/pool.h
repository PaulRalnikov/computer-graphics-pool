#pragma once
#include "../shader_program.h"
#include "../pool_coordinates/pool_coordinates.h"
#include <libs/glm/vec3.hpp>
#include <libs/glm/mat4x4.hpp>

class PoolProgram : public ShaderProgram {
public:
    PoolProgram(std::string vertex_shader_path, std::string fragment_shader_path,
                PoolCoordinates coordinates, GLuint bottom_texture, GLuint wall_texture); 

    void set_model(glm::mat4 model);
    void set_view(glm::mat4 model);
    void set_projection(glm::mat4 model);
    void set_wall_texture(GLuint new_wall_texture);

    void run() override;
private:
    GLuint vbo;
    GLuint ebo;
    Texture texture;

    GLuint bottom_texture;
    GLuint wall_texture;

    PoolCoordinates coordinates;

    GLuint model_location;
    GLuint view_location;
    GLuint projection_location;

    int bottom_vertex_segment_start;
    int bottom_vertex_segment_length;

    int wall_vertex_segment_start;
    int wall_vertex_segment_length;
};