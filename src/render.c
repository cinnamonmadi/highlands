#include "render.h"
#include "global.h"
#include <stdio.h>

const SDL_Color COLOR_WHITE = (SDL_Color){ .r = 255, .g = 255, .b = 255, .a = 255 };

const int RENDER_POSITION_CENTERED = -1;

TTF_Font* debug_font;

typedef struct SpriteTexture {
    SDL_Texture* texture;
    int width;
    int height;
} SpriteTexture;
SpriteTexture* sprite_textures;

// Resource loading functions

bool render_init() {
    debug_font = TTF_OpenFont("./res/hack.ttf", 10);
    if(debug_font == NULL) {
        printf("Unable to initialize font! SDL Error: %s\n", TTF_GetError());
        return false;
    }

    sprite_textures = malloc(SPRITE_COUNT * sizeof(SpriteTexture));
    for(int i = 0; i < SPRITE_COUNT; i++) {
        SDL_Surface* loaded_surface = IMG_Load(sprite_data[i].path);
        if(loaded_surface == NULL) {
            printf("Unable to load texture image! SDL Error: %s\n", IMG_GetError());
            return false;
        }

        sprite_textures[i].texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        if(sprite_textures[i].texture == NULL) {
            printf("Unable to create sprite texture! SDL Error: %s\n", SDL_GetError());
            return false;
        }

        sprite_textures[i].width = loaded_surface->w;
        sprite_textures[i].height = loaded_surface->h;

        SDL_FreeSurface(loaded_surface);
    }

    return true;
}

void render_quit() {
    TTF_CloseFont(debug_font);

    for(int i = 0; i < SPRITE_COUNT; i++) {
        SDL_DestroyTexture(sprite_textures[i].texture);
    }
    free(sprite_textures);
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

// Sprite rendering functions

void render_sprite(Sprite sprite, int x, int y) {
    render_sprite_frame(sprite, 0, x, y);
}

void render_sprite_frame(Sprite sprite, int frame, int x, int y) {
    int frame_long_x = sprite_data[sprite].frame_size[0] * frame;
    SDL_Rect source_rect = (SDL_Rect){
        .x = frame_long_x % sprite_textures[sprite].width,
        .y = frame_long_x / sprite_textures[sprite].width,
        .w = sprite_data[sprite].frame_size[0],
        .h = sprite_data[sprite].frame_size[1]
    };
    SDL_Rect dest_rect = (SDL_Rect) {
        .x = x,
        .y = y,
        .w = source_rect.w,
        .h = source_rect.h
    };

    SDL_RenderCopy(renderer, sprite_textures[sprite].texture, &source_rect, &dest_rect);
}

void render_sprite_animation(Animation animation, int x, int y) {
    render_sprite_frame(animation.sprite, animation.frame, x, y);
}
