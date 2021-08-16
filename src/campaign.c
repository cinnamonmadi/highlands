#include "campaign.h"

#include "global.h"
#include <stdlib.h>

// Campaign state constants
const int TILE_SIZE = 32;
const int CAMERA_SPEED = 300;

// Unit constants
const int UNIT_CAPACITY = 256;
const int UNIT_SPEED = 150;
const int UNIT_MIN_TARGET_DISTANCE = 10;

// Campaign state functions

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

    state.drag_start = VEC2_NULL;
    state.drag_end = VEC2_NULL;

    state.units = malloc(UNIT_CAPACITY * sizeof(Unit));
    state.units[0] = unit_init((vec2){ .x = 10, .y = 10 });
    state.units[1] = unit_init((vec2){ .x = 40, .y = 25 });
    state.units[2] = unit_init((vec2){ .x = 25, .y = 40 });
    state.unit_size = 3;

    return state;
}

void campaign_state_free(CampaignState* state) {
    for(int y = 0; y < state->map_height; y++){
        free(state->map[y]);
    }
    free(state->map);

    free(state->units);
}

void campaign_state_handle_input(CampaignState* state, SDL_Event e) {
    if(e.type == SDL_MOUSEMOTION) {
        state->camera_velocity = VEC2_ZERO;

        if(!vec2_equals(state->drag_start, VEC2_NULL)) {
            state->drag_end = (vec2) { .x = e.motion.x, .y = e.motion.y };
            return;
        }

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

    } else if(e.type == SDL_MOUSEBUTTONDOWN) {
        if(e.button.button == SDL_BUTTON_LEFT) {
            state->drag_start = (vec2) { .x = e.button.x, .y = e.button.y };
            state->drag_end = state->drag_start;
        }

    }else if(e.type == SDL_MOUSEBUTTONUP) {
        if(e.button.button == SDL_BUTTON_LEFT) {
            state->drag_start = VEC2_NULL;
            state->drag_end = VEC2_NULL;
        }
    }
}

void campaign_state_update(CampaignState* state, float delta) {
    state->camera_position = vec2_sum(state->camera_position, vec2_scale(state->camera_velocity, delta));
    state->camera_position = vec2_clamp(state->camera_position, VEC2_ZERO, state->camera_max);

    for(int i = 0; i < state->unit_size; i++) {
        unit_update(&state->units[i], delta);
    }
}

// Unit functions

Unit unit_init(vec2 position) {
    return (Unit) {
        .position = position,
        .target = VEC2_NULL,
        .animation = sprite_animation_init(SPRITE_PRINCE)
    };
}

void unit_update(Unit* unit, float delta) {
    if(!vec2_equals(unit->target, VEC2_NULL)) {
        vec2 velocity = vec2_scale(vec2_direction(unit->position, unit->target), UNIT_SPEED);
        unit->position = vec2_sum(unit->position, vec2_scale(velocity, delta));

        if(vec2_distance(unit->position, unit->target) <= UNIT_MIN_TARGET_DISTANCE) {
            unit->target = VEC2_NULL;
        }
    }
}
