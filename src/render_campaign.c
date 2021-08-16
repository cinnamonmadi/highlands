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
}

void render_map(CampaignState* state) {
    vec2 render_start = (vec2) {
        .x = (int)(state->camera_position.x / TILE_SIZE),
        .y = (int)(state->camera_position.y / TILE_SIZE)
    };
    int top_tile_height = ((render_start.y * TILE_SIZE) - state->camera_position.y) + TILE_SIZE; // Calculates the actual height rendered of the topmost row of visible tiles
    vec2 render_size = (vec2) {
        .x = ceil(SCREEN_WIDTH / (float)TILE_SIZE) + (int)(state->camera_position.x % TILE_SIZE != 0),
        .y = ceil((SCREEN_HEIGHT - top_tile_height) / (float)TILE_SIZE) + 1
    };

    for(int y = render_start.y; y < render_start.y + render_size.y; y++){
        for(int x = render_start.x; x < render_start.x + render_size.x; x++){
            vec2 render_pos = vec2_sub(
                                vec2_scale((vec2) { .x = x, .y = y }, TILE_SIZE),
                                state->camera_position);
            render_sprite_frame(SPRITE_TILESET, state->map[y][x], render_pos);
        }
    }
}

void render_units(CampaignState* state) {
    for(int i = 0; i < state->unit_size; i++) {
        vec2 unit_render_pos = vec2_sub(state->units[i].position, state->camera_position);
        const SpriteData* unit_sprite_data = &sprite_data[state->units[i].animation.sprite];

        SDL_Rect unit_rect = (SDL_Rect) { .x = unit_render_pos.x, .y = unit_render_pos.y, .w = unit_sprite_data->frame_size[0], .h = unit_sprite_data->frame_size[1] };
        if(is_rect_in_screen(unit_rect)) {
            render_sprite_animation(state->units[i].animation, unit_render_pos);
        }
    }
}
