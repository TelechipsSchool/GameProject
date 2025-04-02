#include "display.h"

ALLEGRO_BITMAP* load_bitmap_resized(const char* filename, int w, int h) {
    ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
    if (!loaded_bmp) {
        printf("load_bitmap - �ε� ����: %s", filename);//�̰� DEBUG�޼��� ��� X
        return NULL;
    }

    ALLEGRO_BITMAP* resized_bmp = al_create_bitmap(w, h);

    if (!resized_bmp) {
        DEBUG_MSG(create_bitmap - ���� ����);
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