#pragma once

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

class WindowSize
{
public:
    int width, height;

    WindowSize(SDL_Window *window);

    WindowSize(int width, int height);

    void resize(int new_width, int new_height);
};