#include "event_handler.h"

EventHandler::EventHandler(WindowSize &window_size, WindowSettings &settings, Camera &camera):
    window_size(window_size),
    settings(settings),
    camera(camera)
{
    button_down = {};
};

void EventHandler::handleEvents(float dt)
{
    for (SDL_Event event; SDL_PollEvent(&event);)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            settings.quit();
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                window_size.resize(event.window.data1, event.window.data2);
                break;
            }
            break;
        case SDL_KEYDOWN:
            button_down[event.key.keysym.sym] = true;
            if (event.key.keysym.sym == SDLK_b)
                settings.change_backgound_texture();
            if (event.key.keysym.sym == SDLK_p)
                settings.change_pool_wall_texture();
            if (event.key.keysym.sym == SDLK_SPACE)
                settings.change_time_paused();
            break;
        case SDL_KEYUP:
            button_down[event.key.keysym.sym] = false;
            break;
        }
    }

    // camera events
    if (button_down[SDLK_UP])
        camera.rotate_down(dt);
    if (button_down[SDLK_DOWN])
        camera.rotate_up(dt);

    if (button_down[SDLK_LEFT])
        camera.rotate_left(2.f * dt);
    if (button_down[SDLK_RIGHT])
        camera.rotate_right(2.f * dt);

    if (button_down[SDLK_LSHIFT]){
        camera.set_big_speed();
    }
    else{
        camera.set_normal_speed();
    }

    glm::vec3 move_direction(0.0f);
    if (button_down[SDLK_w])
    {
        move_direction += glm::vec3(0.0f, 0.0f, -1.0f);
    }
    if (button_down[SDLK_s])
    {
        move_direction += glm::vec3(0.0f, 0.0f, 1.0f);
    }
    if (button_down[SDLK_d])
    {
        move_direction += glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if (button_down[SDLK_a])
    {
        move_direction += glm::vec3(-1.0f, 0.0f, 0.0f);
    }

    if (move_direction != glm::vec3(0.0f))
    {
        camera.go(move_direction, dt);
    }
}