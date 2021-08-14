#include "render.h"
#include "global.h"
#include <stdio.h>

const SDL_Color COLOR_WHITE = (SDL_Color){ .r = 255, .g = 255, .b = 255, .a = 255 };

const int RENDER_POSITION_CENTERED = -1;

TTF_Font* debug_font;

bool render_init() {
    debug_font = TTF_OpenFont("./res/hack.ttf", 10);
    if(debug_font == NULL) {
        printf("Unable to initialize font! SDL Error: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void render_quit() {
    TTF_CloseFont(debug_font);
}

void render_clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void render_present() {
    SDL_RenderPresent(renderer);
}

void render_text(const char* text, SDL_Color color, int x, int y) {
    SDL_Surface* text_surface = TTF_RenderText_Solid(debug_font, text, color);
    if(text_surface == NULL) {
        printf("Unable to render text to surface! SDL Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if(text_texture == NULL) {
        printf("Unable to create text texture! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_Rect source_rect = (SDL_Rect){ .x = 0, .y = 0, .w = text_surface->w, .h = text_surface->h };
    SDL_Rect dest_rect = (SDL_Rect){ .x = x, .y = y, .w = text_surface->w, .h = text_surface->h };
    if(dest_rect.x == RENDER_POSITION_CENTERED) {
        dest_rect.x = (SCREEN_WIDTH / 2) - (source_rect.w / 2);
    }
    if(dest_rect.y == RENDER_POSITION_CENTERED) {
        dest_rect.y = (SCREEN_HEIGHT / 2) - (source_rect.h / 2);
    }

    SDL_RenderCopy(renderer, text_texture, &source_rect, &dest_rect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}
