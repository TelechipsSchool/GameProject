#include "audio.h"

void play_music(char* music_path) {
    ALLEGRO_SAMPLE_ID id;
    ALLEGRO_SAMPLE* music = al_load_sample(music_path);
    if (!music) {
        DEBUG_MSG(music - 음향 로드 실패);
        return;
    }
    al_play_sample(music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &id);
}