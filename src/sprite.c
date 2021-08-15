#include "sprite.h"

#include "global.h"

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
    }
};

Animation sprite_animation_init(Sprite sprite) {
    return (Animation) {
        .sprite = sprite,
        .frame = 0,
        .timer = 0,
        .frame_duration = 1.0f / sprite_data[sprite].fps
    };
}

void sprite_animation_update(Animation* animation, float delta) {
    animation->timer += delta;
    if(animation->timer >= animation->frame_duration) {
        animation->frame++;
        animation->timer -= animation->frame_duration;

        if(animation->frame == sprite_data[animation->sprite].frame_count) {
            animation->frame = 0;
        }
    }
}
