#include "audio.h"

void play_music(char* music_path) {
    ALLEGRO_SAMPLE_ID id;
    ALLEGRO_SAMPLE* music = al_load_sample(music_path);
    if (!music) {
        printf("���� �ε� ����!\n");
        return;
    }
    al_play_sample(music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &id);
}