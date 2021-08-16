#include "global.h"
#include "sprite.h"
#include "render.h"
#include "render_campaign.h"
#include "campaign.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

// Game state variables
CampaignState state;

void input();
void render();

bool engine_init(int argc, char** argv);
void engine_quit();
void engine_set_resolution(int width, int height);
void engine_toggle_fullscreen();
void engine_clock_tick();

int main(int argc, char** argv) {

    if(!engine_init(argc, argv)) {
        return 0;
    }

    state = campaign_state_init();

    while(engine_running) {
        input();

        campaign_state_update(&state, delta);

        render();
        engine_clock_tick();
    }

    campaign_state_free(&state);

    return 0;
}

void input() {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            engine_running = false;
        } else if(SDL_GetWindowGrab(window) == SDL_TRUE) {
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                SDL_SetWindowGrab(window, SDL_FALSE);
            } else {
                campaign_state_handle_input(&state, e);
            }
        } else {
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                SDL_SetWindowGrab(window, SDL_TRUE);
            }
        }
    }
}

// Game loop functions
void render() {
    render_clear();

    render_campaign_state(&state);

    // Render FPS
    char fps_text[32];
    sprintf(fps_text, "FPS: %i DPS: %f", fps, dps);
    render_text(fps_text, COLOR_WHITE, 0, 0);

    render_present();
}

// Engine functions

bool engine_init(int argc, char** argv) {
    bool init_fullscreened = false;
    int resolution_width = 640;
    int resolution_height = 360;

    // Parse system arguments
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--fullscreen") == 0) {
            init_fullscreened = true;
        } else if(strcmp(argv[i], "--resolution") == 0) {
            if(i + 1 == argc) {
                printf("No resolution was specified!\n");
                return false;
            }
            i++;

            int arg_index = 0;
            int buffer_index = 0;
            char screen_width_buffer[8];
            char screen_height_buffer[8];

            // Expects format <SCREEN_WIDTH>x<SCREEN_HEIGHT>
            // Traverse the argument character by character until we find the 'x', placing each character we pass into the screen width buffer
            while(argv[i][arg_index] != 'x' && argv[i][arg_index] != '\0') {
                if(buffer_index == 7 || argv[i][arg_index] < '0' || argv[i][arg_index] > '9') {
                    printf("Incorrect resolution format! %c\n", argv[i][arg_index]);
                    return false;
                }
                screen_width_buffer[buffer_index] = argv[i][arg_index];
                buffer_index++;
                arg_index++;
            }
            screen_width_buffer[buffer_index] = '\0';

            // Check that we found an x at all
            if(argv[i][arg_index] == '\0') {
                printf("Incorrect resolution format!\n");
                return false;
            }

            // Traverse the rest of the argument and place each character into the screen height buffer
            arg_index++;
            buffer_index = 0;
            while(argv[i][arg_index] != '\0') {
                if(buffer_index == 7 || argv[i][arg_index] < '0' || argv[i][arg_index] > '9') {
                    printf("Incorrect resolution format!\n");
                    return false;
                }
                screen_height_buffer[buffer_index] = argv[i][arg_index];
                buffer_index++;
                arg_index++;
            }
            screen_height_buffer[buffer_index] = '\0';

            resolution_width = atoi(screen_width_buffer);
            resolution_height = atoi(screen_height_buffer);
        }
    }

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
