#include "campaign.hpp"

#include "global.hpp"
#include <iostream>

const int TILE_SIZE = 32;
const int CAMERA_SPEED = 300;

Campaign::Campaign() {
    mouse_pos = VEC2_ZERO;
    drag_start = VEC2_NULL;
    drag_end = VEC2_NULL;

    map_width = 40;
    map_height = 20;

    map = new unsigned int*[map_height];
    map_collider = new bool*[map_height];
    for(int y = 0; y < map_height; y++) {
        map[y] = new unsigned int[map_width];
        map_collider[y] = new bool[map_width];
        for(int x = 0; x < map_width; x++) {
            map[y][x] = 0;
            map_collider[y][x] = false;

            if(x == 0 || y == 0 || x == map_width - 1 || y == map_height - 1) {
                map[y][x] = 1;
            }
        }
    }

    camera_position = VEC2_ZERO;
    camera_velocity = VEC2_ZERO;
    camera_max = vec2((TILE_SIZE * map_width) - SCREEN_WIDTH, (TILE_SIZE * map_height) - SCREEN_HEIGHT);

    ecs.register_component<vec2>();

    Entity unit = ecs.create_entity();
    Entity obstacle = ecs.create_entity();

    ecs.add_component(unit, vec2(10, 10));
    ecs.add_component(obstacle, vec2(20, 20));

    vec2 unit_position = ecs.get_component<vec2>(unit);
    std::cout << unit_position.x << ", " << unit_position.y << std::endl;
    vec2 obstacle_position = ecs.get_component<vec2>(obstacle);
    std::cout << obstacle_position.x << ", " << obstacle_position.y << std::endl;
}

Campaign::~Campaign() {
    for(int y = 0; y < map_height; y++) {
        delete [] map[y];
        delete [] map_collider[y];
    }
    delete [] map;
    delete [] map_collider;
}

void Campaign::handle_input(SDL_Event e) {
    if(e.type == SDL_MOUSEMOTION) {
        // Set mouse position
        mouse_pos = vec2(e.motion.x, e.motion.y);

        // Update camera velocity
        camera_velocity = VEC2_ZERO;

        if(e.motion.x == 0) {
            camera_velocity.x = -CAMERA_SPEED;
        } else if(e.motion.x == SCREEN_WIDTH - 1) {
            camera_velocity.x = CAMERA_SPEED;
        }
        if(e.motion.y == 0) {
            camera_velocity.y = -CAMERA_SPEED;
        } else if(e.motion.y == SCREEN_HEIGHT - 1) {
            camera_velocity.y = CAMERA_SPEED;
        }

    } else if(e.type == SDL_MOUSEBUTTONDOWN) {
        // Begin drag selecting
        if(e.button.button == SDL_BUTTON_LEFT) {
            drag_start = mouse_pos + camera_position;
        } else if (e.button.button == SDL_BUTTON_RIGHT) {
        }

    }else if(e.type == SDL_MOUSEBUTTONUP) {
        // End drag selecting
        if(e.button.button == SDL_BUTTON_LEFT) {
            drag_start = VEC2_NULL;
            drag_end = VEC2_NULL;
        }
    }
}

void Campaign::update(float delta) {
    camera_position = camera_position + (camera_velocity * delta);
    camera_position = camera_position.clamp(VEC2_ZERO, camera_max);

    if(drag_start != VEC2_NULL){
        drag_end = mouse_pos + camera_position;
    }
}

void Campaign::render() {
    // Render map
    vec2 render_start = vec2((int)((int)camera_position.x / TILE_SIZE), (int)((int)camera_position.y / TILE_SIZE));
    int top_tile_height = ((render_start.y * TILE_SIZE) - (int)camera_position.y) + TILE_SIZE; // Calculates the actual height rendered of the topmost row of visible tiles
    vec2 render_size = vec2(
        ceil(SCREEN_WIDTH / (float)TILE_SIZE) + (int)((int)camera_position.x % TILE_SIZE != 0),
        ceil((SCREEN_HEIGHT - top_tile_height) / (float)TILE_SIZE) + 1);

    for(int y = render_start.y; y < render_start.y + render_size.y; y++){
        for(int x = render_start.x; x < render_start.x + render_size.x; x++){
            vec2 render_pos = (vec2(x, y) * TILE_SIZE) - camera_position;
            render_sprite_frame(SPRITE_TILESET, map[y][x], render_pos);
        }
    }

    // Render drag select box
    if(drag_start != VEC2_NULL) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect drag_rect = to_rect(drag_start - camera_position, drag_end - camera_position);
        SDL_RenderDrawRect(renderer, &drag_rect);
    }
}
