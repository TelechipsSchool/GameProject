#include "help.h"

void help_menu() {
    ALLEGRO_BITMAP* help_screen = al_load_bitmap("images/help.png");
    ALLEGRO_SAMPLE* sample = al_load_sample("audio/help.ogg");
    ALLEGRO_SAMPLE_ID sample_id;    

    // 이벤트 큐 생성
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    if (!sample) {
        printf("음향 로딩 실패!\n");
    }
    else {
        al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &sample_id);

        if (!help_screen) {
            DEBUG_MSG(help_screen - help.png 로딩 실패);
        }
        else {
            al_draw_scaled_bitmap(help_screen, 0, 0,
                al_get_bitmap_width(help_screen), al_get_bitmap_height(help_screen),
                0, 0, SCREEN_W, SCREEN_H, 0);
            al_flip_display();

            // ESC가 눌릴 때까지 기다리는 루프
            bool in_help = true;
            while (in_help) {
                ALLEGRO_EVENT help_event;
                al_wait_for_event(event_queue, &help_event);

                if (help_event.type == ALLEGRO_EVENT_KEY_DOWN &&
                    help_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    in_help = false; // ESC 누르면 루프 종료
                    play_music("audio/cancel.ogg");
                }
            }

            al_stop_sample(&sample_id);
            al_destroy_sample(sample);
            al_destroy_bitmap(help_screen);
        }
    }
}