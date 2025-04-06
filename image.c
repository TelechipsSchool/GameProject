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
        *(assets[i].target) = load_bitmap_resized(assets[i].path, assets[i].width, assets[i].height);
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