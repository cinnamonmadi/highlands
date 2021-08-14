#pragma once

typedef enum Sprite {
    SPRITE_PRINCE,
    SPRITE_COUNT
} Sprite;

typedef struct SpriteData {
    const char* path;
    const int frame_size[2];
    const int frame_count;
    const int fps;
} SpriteData;

typedef struct Animation {
    Sprite sprite;
    int frame;
    float timer;
    float frame_duration;
} Animation;

extern const SpriteData sprite_data[SPRITE_COUNT];

Animation sprite_animation_init(Sprite sprite);
void sprite_animation_update(Animation* animation, float delta);
