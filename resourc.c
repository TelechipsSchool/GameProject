#include "resource.h"

void init_game_resources(ALLEGRO_TIMER** timer, ALLEGRO_EVENT_QUEUE** queue) {
    *timer = al_create_timer(1.0 / FPS);
    *queue = al_create_event_queue();

    al_register_event_source(*queue, al_get_mouse_event_source());
    al_register_event_source(*queue, al_get_keyboard_event_source());
    al_register_event_source(*queue, al_get_timer_event_source(*timer));
    al_start_timer(*timer);
}

void load_game_sounds(ALLEGRO_SAMPLE** pull, ALLEGRO_SAMPLE** release) {
    *pull = al_load_sample("audio/planet_pull.ogg");
    if (!*pull) DEBUG_MSG(pull_sound - 행성 당기는 소리 로드 실패);
    *release = al_load_sample("audio/planet_push.ogg");
}

void load_scroll_background(ALLEGRO_BITMAP* scroll_frames[]) {

    char path[50];
    for (int i = 0; i < SCROLL_FRAMES; ++i) {
        sprintf(path, "images/scroll_frame_%d.png", i);
        scroll_frames[i] = al_load_bitmap(path);
        if (!scroll_frames[i]) DEBUG_MSG(scroll_frames - 로드 실패);
    }
}

void destroy_game_resources(
    ALLEGRO_BITMAP* center,
    ALLEGRO_BITMAP* gravityfield,
    ALLEGRO_BITMAP* startpoint,
    ALLEGRO_BITMAP* scroll_frames[],
    ALLEGRO_EVENT_QUEUE* queue,
    ALLEGRO_TIMER* timer
) {
    destroy_planet_images(planet_images);

    al_destroy_bitmap(center);
    al_destroy_bitmap(gravityfield);
    al_destroy_bitmap(startpoint);

    for (int i = 0; i < SCROLL_FRAMES; ++i) {
        if (scroll_frames[i]) {
            al_destroy_bitmap(scroll_frames[i]);
            scroll_frames[i] = NULL;
        }
    }

    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
}
