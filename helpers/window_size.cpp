#include "window_size.h"
#include <GL/glew.h>

WindowSize::WindowSize(SDL_Window *window)
{
    SDL_GetWindowSize(window, &width, &height);
}

WindowSize::WindowSize(int width, int height) : width(width), height(height) {}

void WindowSize::resize(int new_width, int new_height)
{
    width = new_width;
    height = new_height;
    glViewport(0, 0, width, height);
}