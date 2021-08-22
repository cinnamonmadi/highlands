#include "sprite.hpp"
#include "global.hpp"

const SpriteData sprite_data[SPRITE_COUNT] = {
    (SpriteData) {
        .path = "./res/gfx/tileset.png",
        .frame_size = { 32, 32 },
        .frame_count = 4,
        .fps = 1
    },
    (SpriteData) {
        .path = "./res/gfx/prince.png",
        .frame_size = { 24, 24 },
        .frame_count = 4,
        .fps = 7
    },
    (SpriteData) {
        .path = "./res/gfx/rock.png",
        .frame_size = { 22, 25 },
        .frame_count = 1,
        .fps = 1
    }
};

Animation::Animation(Sprite sprite) {
    this->sprite = sprite;
    frame = 0;
    timer = 0;
    frame_duration = 1.0f / sprite_data[sprite].fps;
}

void Animation::update(float delta) {
    timer += delta;

    if(timer >= frame_duration) {
        frame++;
        timer -= frame_duration;

        if(frame == sprite_data[sprite].frame_count) {
            frame = 0;
        }
    }
}
