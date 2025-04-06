#include "image.h"
#include "init.h" 

extern ALLEGRO_BITMAP* planet_images[8] = { NULL };


//game.c 이미지로드
void load_planet_images() {
    int sizes[] = { 0, 30, 40, 60, 90, 120, 160, 300 };  // index 0은 사용 안 함

    for (int i = 1; i <= 7; ++i) {
        char path[64];
        sprintf(path, "images/planet_%d.png", i);

        planet_images[i] = load_bitmap_resized(path, sizes[i], sizes[i]);


        if (!planet_images[i]) {
            DEBUG_MSG(행성 이미지 로드 실패);
            continue;

        }
    }
}

void destroy_planet_images(ALLEGRO_BITMAP** planet_images) {
    for (int i = 1; i <= 7; ++i) {
        if (planet_images[i]) {
            al_destroy_bitmap(planet_images[i]);
            planet_images[i] = NULL;
        }
    }
}

//menu.c 이미지 로드

bool load_image_assets(const ImageAsset* assets, int count) {
    for (int i = 0; i < count; ++i) {
        if (assets[i].resize) {
            *(assets[i].target) = load_bitmap_resized(assets[i].path, assets[i].width, assets[i].height);
        } else {
            *(assets[i].target) = al_load_bitmap(assets[i].path);
        }

        if (*(assets[i].target) == NULL) {
            DEBUG_MSG(이미지 로드 실패);
            return false;
        }
    }
    return true;
}




ALLEGRO_BITMAP* load_bitmap_resized(const char* filename, int w, int h) {
    ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
    if (!loaded_bmp) {
        printf("load_bitmap - 로드 실패: %s", filename);//이건 DEBUG메세지 사용 X
        return NULL;
    }

    ALLEGRO_BITMAP* resized_bmp = al_create_bitmap(w, h);

    if (!resized_bmp) {
        DEBUG_MSG(create_bitmap - 생성 실패);
        al_destroy_bitmap(loaded_bmp);
        return NULL;
    }
    ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();

    al_set_target_bitmap(resized_bmp);
    al_draw_scaled_bitmap(loaded_bmp, 0, 0,
        al_get_bitmap_width(loaded_bmp),
        al_get_bitmap_height(loaded_bmp),
        0, 0, w, h, 0);
    al_set_target_bitmap(prev_target);
    al_destroy_bitmap(loaded_bmp);

    return resized_bmp;
}



//game2 images-----------------------------------------------------------------------//
bool load_game2_bitmaps() {
    ImageAsset assets[] = {
        {"gfx/background5.png", 0, 0, false, &background},//true:resize여부
        {"gfx/ship.png", 0, 0, false, &ship},
        {"gfx/large_explosion_2.png", 0, 0, false, &explosion_large},
        {"gfx/large_explosion_0.png", 0, 0, false, &explosion_small},
        {"gfx/large_bullet.png", 0, 0, false, &bulletIMG},
        {"gfx/large_asteroid.png", 0, 0, false, &asteroidIMG_large},
        {"gfx/medium_asteroid.png", 0, 0, false, &asteroidIMG_small},
        {"gfx/ship_invisible7.png", 0, 0, false, &invisible_ship},
        {"gfx/trail.png", 0, 0, false, &trail},
        {"gfx/logo.png", 0, 0, false, &logo},
        {"gfx/alien2.png", 120, 120, true, &alien1_withUFO},
        {"gfx/warning3.png", 310, 100, true, &warning},
        {"gfx/small_bullet.png", 10, 10, true, &alien_bullet},
        {"gfx/blood1.png", 250, 250, true, &alien1_die},
        {"gfx/blood5.png", 200, 200, true, &blood2},
        {"gfx/alien_withoutUFO2.png", 130, 130, true, &alien2_withoutUF0},
        {"gfx/alien6_withoutUFO.png", 130, 130, true, &alien2IMG},
        {"gfx/boss7.png", 320, 200, true, &alien3IMG},
        {"gfx/warning_alien3_2.png", 620, 100, true, &warning_alien3},
        {"gfx/large_explosion_1.png", 200, 200, true, &boss_explosion},
        {"gfx/bullet3.png", 20, 20, true, &boss_bullet},
        {"images/heart.png", 40, 40, true, &heart},
        {"images/empty_heart.png", 40, 40, true, &empty_heart}
    };

    return load_image_assets(assets, sizeof(assets) / sizeof(assets[0]));
}

void destroy_game2_bitmaps() {
    al_destroy_bitmap(background);
    al_destroy_bitmap(ship);
    al_destroy_bitmap(explosion_large);
    al_destroy_bitmap(explosion_small);
    al_destroy_bitmap(bulletIMG);
    al_destroy_bitmap(asteroidIMG_large);
    al_destroy_bitmap(asteroidIMG_small);
    al_destroy_bitmap(invisible_ship);
    al_destroy_bitmap(trail);
    al_destroy_bitmap(logo);
    al_destroy_bitmap(alien1_withUFO);
    al_destroy_bitmap(warning);
    al_destroy_bitmap(alien_bullet);
    al_destroy_bitmap(alien1_die);
    al_destroy_bitmap(blood2);
    al_destroy_bitmap(alien2_withoutUF0);
    al_destroy_bitmap(alien2IMG);
    al_destroy_bitmap(alien3IMG);
    al_destroy_bitmap(warning_alien3);
    al_destroy_bitmap(boss_explosion);
    al_destroy_bitmap(boss_bullet);
    al_destroy_bitmap(heart);
    al_destroy_bitmap(empty_heart);
}