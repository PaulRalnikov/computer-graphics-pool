#include "window_flags.h"
#include <stdexcept>
#include <string>

WindowSettings::WindowSettings(std::vector<GLuint> texture_source_cycle) :
    running(true),
    time_paused(false),
    texture_source_cycle(texture_source_cycle)
{
    current_texture_index = 0;
    if (current_texture_index > texture_source_cycle.size()) {
        throw std::runtime_error(
            "Texture sourse cycle is too short; its length is " + std::to_string(texture_source_cycle.size())
        );
    }

}

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
    return texture_source_cycle[current_texture_index];
}

void WindowSettings::change_backgound_texture() {
    current_texture_index++;
    if (current_texture_index == texture_source_cycle.size())
        current_texture_index = 0;
}