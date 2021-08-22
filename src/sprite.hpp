#pragma once

typedef enum Sprite {
    SPRITE_TILESET,
    SPRITE_PRINCE,
    SPRITE_ROCK,
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

    Animation(Sprite sprite);
    void update(float delta);
} Animation;

extern const SpriteData sprite_data[SPRITE_COUNT];
