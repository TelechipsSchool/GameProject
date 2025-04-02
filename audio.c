#include "audio.h"

void esc_music() {
    ALLEGRO_SAMPLE_ID cancel_music_id;
    ALLEGRO_SAMPLE* cancel_music = al_load_sample("audio/cancel.ogg");
    if (!cancel_music) {
        printf("음향 로딩 실패!\n");
    }
    al_play_sample(cancel_music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &cancel_music_id);
    al_stop_sample(&cancel_music_id);
}