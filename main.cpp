#include <cmath>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <array>
#include <map>
#include <iterator>
#include <malloc.h>
#include <optional>
#include <tuple>
#include <fstream>

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
#include <libs/glm/ext/matrix_transform.hpp>
#include <libs/glm/ext/matrix_clip_space.hpp>
#include <libs/glm/ext/scalar_constants.hpp>
#include <libs/glm/gtx/string_cast.hpp>

#include <string_view>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <utility>
#include "libs/stb_image/stb_image.h"
#include "libs/tiny_obj_loader/tiny_obj_loader.h"

#include "frame_time_observer.h"
#include "camera.h"
#include "window_size.h"
#include "window_flags.h"
#include "event_handler.h"
#include "programs/shader_program.h"
#include "programs/environment_map/environment_map_program.h"
#include "programs/pool/pool.h"
#include "programs/water/water.h"

const std::string project_root = PROJECT_ROOT;
const std::string programs_dir = project_root + "/programs";
const std::string environmenmt_map_dir = programs_dir + "/environment_map";
const std::string pool_dir = programs_dir + "/pool";
const std::string water_dir = programs_dir + "/water";

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

bool file_exists_test (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    } 
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
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
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

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

    GLuint usual_backgound_texture = load_texture(textures_dir + "/environment_map.jpg");
    GLuint billy_texture = load_texture(textures_dir + "/billy.jpg");
    GLuint bricks_texture = load_texture(textures_dir + "/bricks_diff.png");

    // programs
    EnvironmentMapProgram environment_map_program(
        environmenmt_map_dir + "/vertex_shader.vert",
        environmenmt_map_dir + "/fragment_shader.frag");

    PoolCoordinates pool_coordinates(20.0, 20.0, 20.0, glm::vec3(10.0, -5.0, 10.0));

    PoolProgram pool_program(
        pool_dir + "/vertex_shader.vert",
        pool_dir + "/fragment_shader.frag",
        pool_coordinates,
        bricks_texture,
        bricks_texture);

    WaterProgram water_program(
        water_dir + "/main.vert",
        water_dir + "/main.frag",
        pool_coordinates
    );

    Camera camera(glm::vec3(0.0), -0.2, 1.5);

    WindowSettings settings(usual_backgound_texture, billy_texture);
    EventHandler handler(window_size, settings, camera);
    

    while (settings.get_running())
    {
        float dt = observer.start_frame();
        handler.handleEvents(dt);
        
        if (!settings.get_time_paused()) time += dt;

        glm::mat4 view = camera.get_view();
        glm::vec3 camera_position = camera.get_position();

        float near = 0.1f;
        float far = 100.f;

        glm::mat4 model(1.f);
        glm::mat4 projection = glm::perspective(glm::pi<float>() / 2.f, (window_size.width * 1.f) / window_size.height, near, far);
        glm::mat4 view_inverse = glm::inverse(projection * view);
        glm::vec3 sun_direction = glm::normalize(glm::vec3(std::sin(time * 0.5f), 1.f, std::cos(time * 0.5f)));

        environment_map_program.set_camera_position(camera_position);
        environment_map_program.set_view_inverse(view_inverse);
        environment_map_program.set_environment_texture(settings.get_backgound_texture());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDisable(GL_DEPTH_TEST);
        environment_map_program.run();

        pool_program.set_model(model);
        pool_program.set_projection(projection);
        pool_program.set_view(view);

        glEnable(GL_DEPTH_TEST);
        pool_program.run();

        water_program.set_model(model);
        water_program.set_projection(projection);
        water_program.set_view(view);
        water_program.fetch_time(time);
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
