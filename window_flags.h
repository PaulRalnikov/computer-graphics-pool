#pragma once
#include <GL/glew.h>

class WindowSettings
{
public:
    WindowSettings(GLuint usual_texture, GLuint extra_texture);

    void quit();

    void change_time_paused();

    bool get_running() const;

    bool get_time_paused() const;

    GLuint get_backgound_texture() const;

    void change_backgound_texture();

private:
    bool running;
    bool time_paused;
    GLuint backgound_texture;

    GLuint usual_texture;
    GLuint extra_texture;
};
