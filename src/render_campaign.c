#include "render_campaign.h"

#include "global.h"
#include "render.h"
#include "sprite.h"
#include "vector.h"
#include <math.h>

void render_map(CampaignState* state);

void render_campaign_state(CampaignState* state) {
    render_map(state);
}

void render_map(CampaignState* state) {
    vec2 render_start = (vec2) {
        .x = (int)(state->camera_position.x / TILE_SIZE),
        .y = (int)(state->camera_position.y / TILE_SIZE)
    };
    vec2 render_size = (vec2) {
        .x = ceil(SCREEN_WIDTH / (float)TILE_SIZE) + (int)(state->camera_position.x % TILE_SIZE == 0),
        .y = ceil(SCREEN_HEIGHT / (float)TILE_SIZE) + (int)(state->camera_position.y % TILE_SIZE == 0)
    };

    for(int y = render_start.y; y < render_size.y; y++){
        for(int x = render_start.x; x < render_size.x; x++){
            vec2 render_pos = vec2_sub(
                                vec2_scale((vec2) { .x = x, .y = y }, TILE_SIZE),
                                state->camera_position);
            render_sprite_frame(SPRITE_TILESET, state->map[y][x], render_pos);
        }
    }
}
