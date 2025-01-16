#include "window_flags.h"

WindowSettings::WindowSettings(GLuint usual_texture, GLuint extra_texture) :
    running(true),
    time_paused(false),
    usual_texture(usual_texture),
    extra_texture(extra_texture),
    backgound_texture(usual_texture)
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
    return backgound_texture;
}

void WindowSettings::change_backgound_texture() {
    if (backgound_texture == usual_texture)
        backgound_texture = extra_texture;
    else 
        backgound_texture = usual_texture;
}