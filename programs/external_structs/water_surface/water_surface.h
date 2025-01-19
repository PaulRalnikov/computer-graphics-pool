#pragma once
#include "programs/external_structs/pool_coordinates/pool_coordinates.h"
#include <GL/glew.h>

class WaterSurface {
public:
    WaterSurface(const PoolCoordinates& coordinates, size_t quality);

    void fetch_time(float time);
    void bind_buffers() const;
    size_t get_indexes_count() const;
    const PoolCoordinates& get_pool_coordinates() const;
    
private:
    const PoolCoordinates& coordinates;
    size_t quality;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};