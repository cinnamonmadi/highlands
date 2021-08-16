#include "campaign.h"

#include "global.h"
#include <stdlib.h>

const int TILE_SIZE = 32;
const int CAMERA_SPEED = 300;

CampaignState campaign_state_init() {
    CampaignState state;

    state.map_width = 40;
    state.map_height = 20;

    state.map = malloc(state.map_height * sizeof(int*));
    for(int y = 0; y < state.map_height; y++) {
        state.map[y] = malloc(state.map_width * sizeof(int));
        for(int x = 0; x < state.map_width; x++) {
            state.map[y][x] = 0;
            if(x == 0 || y == 0 || x == state.map_width - 1 || y == state.map_height - 1) {
                state.map[y][x] = 1;
            }
        }
    }

    state.camera_position = VEC2_ZERO;
    state.camera_velocity = VEC2_ZERO;
    state.camera_max = (vec2) { .x = (TILE_SIZE * state.map_width) - SCREEN_WIDTH, .y = (TILE_SIZE * state.map_height) - SCREEN_HEIGHT };

    return state;
}

void campaign_state_handle_input(CampaignState* state, SDL_Event e) {
    if(e.type == SDL_MOUSEMOTION) {
        state->camera_velocity = VEC2_ZERO;

        if(e.motion.x == 0) {
            state->camera_velocity.x = -CAMERA_SPEED;
        } else if(e.motion.x == SCREEN_WIDTH - 1) {
            state->camera_velocity.x = CAMERA_SPEED;
        }
        if(e.motion.y == 0) {
            state->camera_velocity.y = -CAMERA_SPEED;
        } else if(e.motion.y == SCREEN_HEIGHT - 1) {
            state->camera_velocity.y = CAMERA_SPEED;
        }
    }
}

void campaign_state_update(CampaignState* state, float delta) {
    state->camera_position = vec2_sum(state->camera_position, vec2_scale(state->camera_velocity, delta));
    state->camera_position = vec2_clamp(state->camera_position, VEC2_ZERO, state->camera_max);
}
