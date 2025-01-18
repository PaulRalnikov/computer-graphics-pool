#pragma once

#include <map>
#include "window_size.h"
#include "window_flags.h"
#include "camera.h"

class EventHandler
{
public:
    EventHandler(WindowSize &window_size, WindowSettings &flags, Camera &camera);

    void handleEvents(float dt);

private:
    std::map<SDL_Keycode, bool> button_down;
    WindowSize &window_size;
    WindowSettings &settings;
    Camera &camera;
};
