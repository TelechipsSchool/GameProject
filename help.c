#include "help.h"

void help_menu() {
    ALLEGRO_BITMAP* help_screen = al_load_bitmap("images/help.png");
    ALLEGRO_BITMAP* help_screen2 = al_load_bitmap("images/help2.png");
    ALLEGRO_SAMPLE* sample = al_load_sample("audio/help.ogg");

    // 이벤트 큐 생성
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    if (!sample) {
        DEBUG_MSG(sample - help 음향 로드 실패);
        return;
    }
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

    if (!help_screen) {
        DEBUG_MSG(help_screen - help.png 로딩 실패);
        return;
    }
    al_draw_scaled_bitmap(help_screen, 0, 0,
        al_get_bitmap_width(help_screen), al_get_bitmap_height(help_screen),
        0, 0, SCREEN_W, SCREEN_H, 0);
    al_flip_display();

    // ESC가 눌릴 때까지 기다리는 루프
    bool in_help = true;
    while (in_help) {
        ALLEGRO_EVENT help_event;
        al_wait_for_event(event_queue, &help_event);

        if (help_event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (help_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                in_help = false; // ESC 누르면 루프 종료
                al_stop_samples();
                play_music("audio/cancel.ogg");
                menu();
            }

            if (help_event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                al_draw_scaled_bitmap(help_screen2, 0, 0,
                    al_get_bitmap_width(help_screen2), al_get_bitmap_height(help_screen2),
                    0, 0, SCREEN_W, SCREEN_H, 0);
                al_flip_display();
            }

            if (help_event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                al_draw_scaled_bitmap(help_screen, 0, 0,
                    al_get_bitmap_width(help_screen), al_get_bitmap_height(help_screen),
                    0, 0, SCREEN_W, SCREEN_H, 0);
                al_flip_display();
            }
        }        
    }
    al_destroy_sample(sample);
    al_destroy_bitmap(help_screen);
    al_destroy_bitmap(help_screen2);
}