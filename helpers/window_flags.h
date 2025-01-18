#pragma once
#include <vector>
#include <GL/glew.h>

class WindowSettings
{
public:
    WindowSettings(std::vector<GLuint> texture_sourse_cycle);

    void quit();

    void change_time_paused();

    bool get_running() const;

    bool get_time_paused() const;

    GLuint get_backgound_texture() const;

    void change_backgound_texture();

private:
    bool running;
    bool time_paused;

    std::vector<GLuint> texture_source_cycle;
    unsigned int current_texture_index;
};
