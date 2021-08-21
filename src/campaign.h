#pragma once

#include "vector.h"
#include "sprite.h"
#include <SDL2/SDL.h>

typedef struct Unit {
    vec2 position;
    vec2 target;
    Animation animation;
    bool is_selected;
} Unit;

typedef struct CampaignState {
    vec2 mouse_pos;
    vec2 drag_start;
    vec2 drag_end;

    unsigned int** map; // map rows are y, columns are x, so any index is map[y][x]
    int map_width;
    int map_height;

    vec2 camera_position;
    vec2 camera_velocity;
    vec2 camera_max;

    Unit* units;
    int unit_size;
} CampaignState;

CampaignState campaign_state_init();
void campaign_state_free(CampaignState* state);
void campaign_state_handle_input(CampaignState* state, SDL_Event e);
void campaign_state_update(CampaignState* state, float delta);
void campaign_state_select(CampaignState* state);

void campaign_state_update_units(CampaignState* state, float delta);

Unit unit_init(vec2 position);
SDL_Rect unit_rect(Unit* unit);
