#include "window_flags.h"
#include <stdexcept>
#include <string>

TextureCycle::TextureCycle(std::vector<GLuint> texture_cycle):
    texture_cycle(texture_cycle), current_texture_index(0) {}

GLuint TextureCycle::get_current_texture() const
{
    return texture_cycle[current_texture_index];
}

void TextureCycle::change_texture() {
    current_texture_index++;
    if (current_texture_index == texture_cycle.size())
        current_texture_index = 0;
}

WindowSettings::WindowSettings(
    std::vector<GLuint> backgound_texture_source_cycle,
    std::vector<GLuint> pool_wall_texture_source_cycle
) :
    running(true),
    time_paused(false),
    backgound_texture_cycle(backgound_texture_source_cycle),
    pool_wall_texture_cycle(pool_wall_texture_source_cycle)
{}

void WindowSettings::quit()
{
    running = false;
}

void WindowSettings::change_time_paused()
{
    time_paused = 1 - time_paused;
}

bool WindowSettings::get_running() const
{
    return running;
}

bool WindowSettings::get_time_paused() const
{
    return time_paused;
}

GLuint WindowSettings::get_backgound_texture() const {
    return backgound_texture_cycle.get_current_texture();
}

GLuint WindowSettings::get_pool_wall_texture() const {
    return pool_wall_texture_cycle.get_current_texture();
}

void WindowSettings::change_backgound_texture() {
    backgound_texture_cycle.change_texture();
}

void WindowSettings::change_pool_wall_texture()
{
    pool_wall_texture_cycle.change_texture();
}