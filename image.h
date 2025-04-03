#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> 

typedef struct _ImageAsset {
    const char* path;
    int width;
    int height;
    ALLEGRO_BITMAP** target;
} ImageAsset;

ALLEGRO_BITMAP* planet_images[8];
void load_planet_images();
void destroy_planet_images(ALLEGRO_BITMAP** planet_images);
bool load_image_assets(const ImageAsset* assets, int count);