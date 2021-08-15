#include "campaign.h"

#include "global.h"
#include <stdlib.h>

const int TILE_SIZE = 32;

CampaignState campaign_state_init() {
    CampaignState state;

    state.camera_position = VEC2_ZERO;

    state.map_width = 40;
    state.map_height = 20;

    state.map = malloc(state.map_height * sizeof(int*));
    for(int y = 0; y < state.map_height; y++) {
        state.map[y] = malloc(state.map_width * sizeof(int));
        for(int x = 0; x < state.map_width; x++) {
            state.map[y][x] = 0;
        }
    }

    return state;
}
