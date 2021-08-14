#include "global.h"
#include "sprite.h"
#include "render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

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

Animation prince_animation;

void render();

bool engine_init();
void engine_quit();
void engine_set_resolution(int width, int height);
void engine_toggle_fullscreen();
void engine_clock_tick();

int main() {

    if(!engine_init()) {
        return 0;
    }

    prince_animation = sprite_animation_init(SPRITE_PRINCE);
    while(engine_running) {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                engine_running = false;
            }
        }

        sprite_animation_update(&prince_animation, delta);

        render();
        engine_clock_tick();
    }
}

// Game loop functions
void render() {
    render_clear();

    render_sprite_animation(prince_animation, 10, 10);

    // Render FPS
    char fps_text[32];
    sprintf(fps_text, "FPS: %i DPS: %f", fps, dps);
    render_text(fps_text, COLOR_WHITE, 0, 0);

    render_present();
}

// Engine functions

bool engine_init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Unable to initialize SDL! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Highlands", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int img_flags = IMG_INIT_PNG;

    if(!(IMG_Init(img_flags) & img_flags)){
        printf("Unable to initialize SDL_image! SDL Error: %s\n", IMG_GetError());
        return false;
    }

    if(TTF_Init() == -1){
        printf("Unable to initialize SDL_ttf! SDL Error: %s\n", TTF_GetError());
        return false;
    }

    if(!window || !renderer){
        printf("Unable to initialize engine!\n");
        return false;
    }

    engine_set_resolution(1280, 720);

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
