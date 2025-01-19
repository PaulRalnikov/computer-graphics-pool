#pragma once
#include <vector>
#include <GL/glew.h>

class TextureCycle{
public:
    TextureCycle(std::vector<GLuint> texture_cycle);

    void change_texture();
    GLuint get_current_texture() const;
private:
    std::vector<GLuint> texture_cycle;
    unsigned int current_texture_index;
};

class WindowSettings
{
public:
    WindowSettings(
        std::vector<GLuint> backgound_texture_sourse_cycle,
        std::vector<GLuint> pool_wall_texture_sourse_cycle
    );

    void quit();

    void change_time_paused();
    bool get_running() const;
    bool get_time_paused() const;

    GLuint get_backgound_texture() const;
    void change_backgound_texture();

    GLuint get_pool_wall_texture() const;
    void change_pool_wall_texture();

private:
    bool running;
    bool time_paused;

    TextureCycle backgound_texture_cycle;
    TextureCycle pool_wall_texture_cycle;
};
