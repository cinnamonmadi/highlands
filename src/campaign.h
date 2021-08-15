#pragma once

#include "vector.h"

typedef struct CampaignState {
    vec2 camera_position;
    unsigned int** map; // map rows are y, columns are x, so any index is map[y][x]
    int map_width;
    int map_height;
} CampaignState;

CampaignState campaign_state_init();
// void campaign_state_update(CampaignState* state, float delta);
