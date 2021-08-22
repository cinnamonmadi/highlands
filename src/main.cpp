#include "global.hpp"
#include "sprite.hpp"
#include "render.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

// Engine variables
SDL_Window* window;
SDL_Renderer* renderer;

bool engine_running = true;
bool engine_is_fullscreen = false;

// Timing variables
const float FRAME_DURATION = 1.0f / 60.0f;
float last_frame_time = 0.0f;
float last_update_time = 0.0f;
float last_second_time = 0.0f;
int frames = 0;
int fps = 0;
float delta = 0.0f;
float deltas = 0.0f;
float dps = 0.0f;

void input();
void render();

bool engine_init(int argc, char** argv);
void engine_quit();
void engine_set_resolution(int width, int height);
void engine_toggle_fullscreen();
void engine_clock_tick();

Animation prince = Animation(SPRITE_PRINCE);

int main(int argc, char** argv) {
    if(!engine_init(argc, argv)) {
        return 0;
    }

    while(engine_running) {
        input();

        prince.update(delta);

        render();
        engine_clock_tick();
    }

    return 0;
}

// Game loop functions

void input() {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            engine_running = false;
        }
    }
}

void render() {
    render_clear();

    render_sprite_animation(prince, vec2(32, 32));

    render_text(("FPS " + std::to_string(fps) + " DPS " + std::to_string(dps)).c_str(), COLOR_WHITE, 0, 0);
    render_present();
}

// Engine functions

bool engine_init(int argc, char** argv) {
    bool init_fullscreened = false;
    int resolution_width = 1280;
    int resolution_height = 720;

    // Parse system arguments
    for(int i = 1; i < argc; i++) {
        std::string arg = std::string(argv[i]);
        if(arg == "--fullscreen"){
            init_fullscreened = true;
        } else if(arg == "--resolution") {
            if(i + 1 == argc) {
                std::cout << "No resolution was specified!" << std::endl;
                return false;
            }

            i++;
            std::string resolution_input = std::string(argv[i]);

            size_t x_index = resolution_input.find_first_of("x");
            if(x_index == std::string::npos) {
                std::cout << "Incorrect resolution format!" << std::endl;
                return false;
            }

            resolution_width = atoi(resolution_input.substr(0, x_index).c_str());
            resolution_height = atoi(resolution_input.substr(x_index + 1, resolution_input.length() - x_index + 1).c_str());

            if(resolution_width == 0 || resolution_height == 0) {
                std::cout << "Incorrect resolution format!" << std::endl;
                return false;
            }
        }
    }

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "Unable to initialize SDL! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Highlands", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int img_flags = IMG_INIT_PNG;

    if(!(IMG_Init(img_flags) & img_flags)){
        std::cout << "Unable to initialize SDL_image! SDL Error: " << IMG_GetError() << std::endl;
        return false;
    }

    if(TTF_Init() == -1){
        std::cout << "Unable to initialize SDL_ttf! SDL Error: " << TTF_GetError() << std::endl;
        return false;
    }

    if(!window || !renderer){
        std::cout << "Unable to initial engine!" << std::endl;
        return false;
    }

    engine_set_resolution(resolution_width, resolution_height);
    if(init_fullscreened) {
        engine_toggle_fullscreen();
    }

    render_init();

    return true;
}

void engine_quit() {
    render_quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void engine_set_resolution(int width, int height) {
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetWindowSize(window, width, height);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void engine_toggle_fullscreen() {
    if (engine_is_fullscreen){
        SDL_SetWindowFullscreen(window, 0);
    } else {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
    engine_is_fullscreen = !engine_is_fullscreen;
}

void engine_clock_tick() {
    frames++;
    float current_time = SDL_GetTicks() / 1000.0f;

    // If one second has passed, record what the fps and dps was during that second
    if(current_time - last_second_time >= 1.0f) {
        fps = frames;
        dps = deltas;
        frames = 0;
        deltas = 0;
        last_second_time += 1.0;
    }

    // Record the delta time for consistent game logi
    delta = current_time - last_update_time;
    deltas += delta;
    last_update_time = current_time;

    // Delay if there's extra time between frames
    if(current_time - last_frame_time < FRAME_DURATION) {
        unsigned long delay_time = (unsigned long)(1000.0f * (FRAME_DURATION - (current_time - last_frame_time)));
        SDL_Delay(delay_time);
    }
}
