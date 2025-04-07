#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "init.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> 

typedef struct _ImageAsset {
    const char* path;
    int width;
    int height;
    bool resize;
    ALLEGRO_BITMAP** target;
} ImageAsset;

ALLEGRO_BITMAP* planet_images[8];
void load_planet_images();
void destroy_planet_images(ALLEGRO_BITMAP** planet_images);
bool load_image_assets(const ImageAsset* assets, int count);

extern ALLEGRO_DISPLAY* display;
ALLEGRO_BITMAP* load_bitmap_resized(const char* filename, int w, int h);

//game2 images-----------------------------------------------------------------------//
bool load_game2_bitmaps(void);
void destroy_game2_bitmaps();

extern ALLEGRO_BITMAP* background, * ship, * explosion_large, * explosion_small;
extern ALLEGRO_BITMAP* bulletIMG, * asteroidIMG_large, * asteroidIMG_small;
extern ALLEGRO_BITMAP* invisible_ship, * trail, * logo;
extern ALLEGRO_BITMAP* alien1_withUFO, * warning, * alien_bullet, * alien1_die;
extern ALLEGRO_BITMAP* blood2, * alien2_withoutUF0, * alien2IMG, * alien3IMG;
extern ALLEGRO_BITMAP* warning_alien3, * boss_explosion, * boss_bullet;
extern ALLEGRO_BITMAP* heart, * empty_heart;
