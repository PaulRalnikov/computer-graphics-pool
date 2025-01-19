#include <cassert>

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <libs/glm/vec3.hpp>
#include <libs/glm/mat4x4.hpp>
#include <libs/glm/gtx/string_cast.hpp>

#include <string_view>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <vector>

#include "libs/stb_image/stb_image.h"
#include "libs/tiny_obj_loader/tiny_obj_loader.h"

#include "helpers/frame_time_observer.h"
#include "helpers/camera.h"
#include "helpers/window_size.h"
#include "helpers/window_flags.h"
#include "helpers/event_handler.h"
#include "programs/shader_program.h"
#include "programs/environment_map/environment_map_program.h"
#include "programs/pool/pool.h"
#include "programs/water/water.h"
#include "programs/caustics/caustics.h"
#include "programs/external_structs/water_surface/water_surface.h"
#include "programs/external_structs/pool_coordinates/pool_coordinates.h"

const std::string project_root = PROJECT_ROOT;
const std::string programs_dir = project_root + "/programs";
const std::string environmenmt_map_dir = programs_dir + "/environment_map";
const std::string pool_dir = programs_dir + "/pool";
const std::string water_dir = programs_dir + "/water";
const std::string caustics_dir = programs_dir + "/caustics";

const std::string textures_dir = project_root + "/textures";

std::string to_string(std::string_view str)
{
    return std::string(str.begin(), str.end());
}

void sdl2_fail(std::string_view message)
{
    throw std::runtime_error(to_string(message) + SDL_GetError());
}

void glew_fail(std::string_view message, GLenum error)
{
    throw std::runtime_error(to_string(message) + reinterpret_cast<const char *>(glewGetErrorString(error)));
}

void check_error() {
    static int cnt = 0;
    auto error = glGetError();
    if (error != GL_NO_ERROR) {
        std::string err = "No error";
        switch (error) {
            case GL_INVALID_ENUM:
                err = "Invalid enum";
                break;
            case GL_INVALID_VALUE:
                err = "Invalid value";
                break;
            case GL_INVALID_OPERATION:
                err = "Invalid operation";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                err = "Invalid framebuffer operation";
                break;
            case GL_OUT_OF_MEMORY:
                err = "Out of memory";
                break;
        }
        std::cout << "error '" << err << "'; cnt = " << cnt << std::endl;
        assert(false);
    }
    cnt++;
}

GLuint load_texture(std::string const &path)
{
    int width, height, channels;
    auto pixels = stbi_load(path.data(), &width, &height, &channels, 4);

    GLuint result;
    glGenTextures(1, &result);
    glBindTexture(GL_TEXTURE_2D, result);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);

    return result;
}

int main() try
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        sdl2_fail("SDL_Init: ");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window * window = SDL_CreateWindow("Graphics final project",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    if (!window)
        sdl2_fail("SDL_CreateWindow: ");

    WindowSize window_size(window);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context)
        sdl2_fail("SDL_GL_CreateContext: ");

    SDL_GL_SetSwapInterval(0);

    if (auto result = glewInit(); result != GLEW_NO_ERROR)
        glew_fail("glewInit: ", result);

    if (!GLEW_VERSION_3_3)
        throw std::runtime_error("OpenGL 3.3 is not supported");

    glClearColor(0.8f, 0.8f, 1.f, 0.f);  

    FrameTimeObserver observer;

    float time = 0.f;

    GLuint school_texture_source = load_texture(textures_dir + "/school.jpg");
    GLuint billy_texture_source = load_texture(textures_dir + "/billy.jpg");
    GLuint van_texture_source = load_texture(textures_dir + "/van.jpg");
    GLuint forest_texture_source = load_texture(textures_dir + "/forest.jpg");
    GLuint bricks_texture_source = load_texture(textures_dir + "/bricks_diff.png");

    Camera camera(glm::vec3(0.0), -0.2, 1.5);
    WindowSettings settings(
        std::vector<GLuint>({school_texture_source,
                             forest_texture_source,
                             billy_texture_source}),
        std::vector<GLuint>({bricks_texture_source,
                             van_texture_source})
    );
    EventHandler handler(window_size, settings, camera);

    PoolCoordinates pool_coordinates(20.0, 40.0, 20.0, glm::vec3(0.0, -5.0, 0.0));
    WaterSurface water_surface(pool_coordinates, 128);

    // programs
    EnvironmentMapProgram environment_map_program(
        environmenmt_map_dir + "/main.vert",
        environmenmt_map_dir + "/main.frag"
    );
    CausticsProgram caustics_program(
        caustics_dir + "/main.vert",
        caustics_dir + "/main.frag",
        water_surface,
        512
    );
    PoolProgram pool_program(
        pool_dir + "/main.vert",
        pool_dir + "/main.frag",
        pool_coordinates,
        bricks_texture_source
    );
    WaterProgram water_program(
        water_dir + "/main.vert",
        water_dir + "/main.frag",
        water_surface
    );

    while (settings.get_running())
    {
        float dt = observer.start_frame();
        handler.handleEvents(dt);
        
        if (!settings.get_time_paused()) time += dt;

        float near = 0.1f;
        float far = 100.f;

        glm::mat4 model(1.f);
        glm::mat4 projection = glm::perspective(glm::pi<float>() / 2.f, (window_size.width * 1.f) / window_size.height, near, far);
        glm::mat4 view = camera.get_view();
        glm::mat4 view_inverse = glm::inverse(projection * view);
        glm::vec3 sun_direction = glm::normalize(glm::vec3(0.2, 1, 1));
        glm::vec3 camera_position = camera.get_position();

        GLuint backgound_texture_source = settings.get_backgound_texture();
        GLuint pool_wall_texture_source = settings.get_pool_wall_texture();

        glEnable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);
        environment_map_program.set_camera_position(camera_position);
        environment_map_program.set_view_inverse(view_inverse);
        environment_map_program.set_environment_texture(backgound_texture_source);
        environment_map_program.run();

        Rectangle bottom = pool_coordinates.get_bottom();
        caustics_program.set_model(model);
        caustics_program.set_sun_direction(sun_direction);
        caustics_program.set_time(time);
        caustics_program.set_corner(bottom.corner);
        caustics_program.set_x_side_vector(bottom.x_side);
        caustics_program.set_y_side_vector(bottom.y_side);
        caustics_program.run(); 

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        pool_program.set_model(model);
        pool_program.set_projection(projection);
        pool_program.set_view(view);
        pool_program.set_sun_direction(sun_direction);
        pool_program.set_wall_texture(pool_wall_texture_source);
        pool_program.set_caustics_texture(caustics_program.get_caustics_texture_source());
        pool_program.run();

        water_surface.fetch_time(time);

        water_program.set_model(model);
        water_program.set_projection(projection);
        water_program.set_view(view);
        water_program.set_bottom_texture(bricks_texture_source);
        water_program.set_wall_texture(pool_wall_texture_source);
        water_program.set_environment_texture(backgound_texture_source);
        water_program.set_camera_position(camera_position);
        water_program.set_sun_direction(sun_direction);
        water_program.set_time(time);
        water_program.run();

        check_error();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
}
catch (std::exception const & e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
