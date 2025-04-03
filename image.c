#include "image.h"
#include "init.h" 
#include "display.h"

extern ALLEGRO_BITMAP* planet_images[8] = { NULL };


//game.c �̹����ε�
void load_planet_images() {
    int sizes[] = { 0, 30, 40, 60, 90, 120, 160, 300 };  // index 0�� ��� �� ��

    for (int i = 1; i <= 7; ++i) {
        char path[64];
        sprintf(path, "images/planet_%d.png", i);

        planet_images[i] = load_bitmap_resized(path, sizes[i], sizes[i]);


        if (!planet_images[i]) {
            DEBUG_MSG(�༺ �̹��� �ε� ����);
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

//menu.c �̹��� �ε�
bool load_image_assets(const ImageAsset* assets, int count) {
    for (int i = 0; i < count; ++i) {
        *(assets[i].target) = load_bitmap_resized(assets[i].path, assets[i].width, assets[i].height);
        if (*(assets[i].target) == NULL) {
            DEBUG_MSG(�̹��� �ε� ����);
            return false;
        }
    }
    return true;
}
