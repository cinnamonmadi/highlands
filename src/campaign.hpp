#pragma once

#include "render.hpp"
#include "vector.hpp"
#include "ecs.hpp"
#include <SDL2/SDL.h>

class Campaign {
    public:
        Campaign();
        ~Campaign();
        void handle_input(SDL_Event e);
        void update(float delta);
        void render();
    private:
        vec2 mouse_pos;
        vec2 drag_start;
        vec2 drag_end;

        unsigned int** map; // map rows are y, columns are x, so any index is map[y][x]
        bool** map_collider;
        int map_width;
        int map_height;

        vec2 camera_position;
        vec2 camera_velocity;
        vec2 camera_max;

        ECS ecs;
};
