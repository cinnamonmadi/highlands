#pragma once

#include "vector.h"
#include <SDL2/SDL.h>

typedef struct CampaignState {
    unsigned int** map; // map rows are y, columns are x, so any index is map[y][x]
    int map_width;
    int map_height;

    vec2 camera_position;
    vec2 camera_velocity;
    vec2 camera_max;
} CampaignState;

CampaignState campaign_state_init();
void campaign_state_handle_input(CampaignState* state, SDL_Event e);
void campaign_state_update(CampaignState* state, float delta);
