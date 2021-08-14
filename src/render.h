#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

extern const SDL_Color COLOR_WHITE;

extern const int RENDER_POSITION_CENTERED;

extern SDL_Renderer* renderer;

bool render_init();
void render_quit();

void render_clear();
void render_present();

void render_text(const char* text, SDL_Color color, int x, int y);
