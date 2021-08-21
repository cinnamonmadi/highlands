#include "render_campaign.h"

#include "global.h"
#include "render.h"
#include "sprite.h"
#include "vector.h"
#include <math.h>

void render_map(CampaignState* state);
void render_units(CampaignState* state);

void render_campaign_state(CampaignState* state) {
    render_map(state);
    render_units(state);

    // Render drag select box
    if(!vec2_equals(state->drag_start, VEC2_NULL)) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect drag_rect = to_rect(vec2_sub(state->drag_start, state->camera_position), vec2_sub(state->drag_end, state->camera_position));
        SDL_RenderDrawRect(renderer, &drag_rect);
    }
}

void render_map(CampaignState* state) {
    vec2 render_start = (vec2) {
        .x = (int)((int)state->camera_position.x / TILE_SIZE),
        .y = (int)((int)state->camera_position.y / TILE_SIZE)
    };
    int top_tile_height = ((render_start.y * TILE_SIZE) - (int)state->camera_position.y) + TILE_SIZE; // Calculates the actual height rendered of the topmost row of visible tiles
    vec2 render_size = (vec2) {
        .x = ceil(SCREEN_WIDTH / (float)TILE_SIZE) + (int)((int)state->camera_position.x % TILE_SIZE != 0),
        .y = ceil((SCREEN_HEIGHT - top_tile_height) / (float)TILE_SIZE) + 1
    };

    for(int y = render_start.y; y < render_start.y + render_size.y; y++){
        for(int x = render_start.x; x < render_start.x + render_size.x; x++){
            vec2 render_pos = vec2_sub(
                                vec2_mult((vec2) { .x = x, .y = y }, TILE_SIZE),
                                state->camera_position);
            render_sprite_frame(SPRITE_TILESET, state->map[y][x], render_pos);
        }
    }
}

void render_units(CampaignState* state) {
    // Sets the color for the unit selection boxes
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    for(int i = 0; i < state->unit_size; i++) {
        // Get the units screen rectangle
        vec2 unit_render_pos = vec2_sub(state->units[i].position, state->camera_position);
        SDL_Rect render_rect = unit_rect(&state->units[i]);
        unit_render_pos.x = unit_render_pos.x - (render_rect.w / 2);
        unit_render_pos.y = unit_render_pos.y - (render_rect.h / 2);
        render_rect.x = unit_render_pos.x;
        render_rect.y = unit_render_pos.y;

        // Draw the unit's sprite if they are on screen
        if(is_rect_in_screen(render_rect)) {
            render_sprite_animation(state->units[i].animation, unit_render_pos);

            // Draw the selection rect if the unit is selected
            if(state->units[i].is_selected) {
                SDL_RenderDrawRect(renderer, &render_rect);
            }
        }
    }
}
