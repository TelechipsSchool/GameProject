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




ALLEGRO_SAMPLE_INSTANCE* init_bgm(const char* filepath) {
    ALLEGRO_SAMPLE* sample = al_load_sample(filepath);
    if (!sample) {
        DEBUG_MSG(bgm sample load fail);
        return NULL;
    }

    ALLEGRO_SAMPLE_INSTANCE* instance = al_create_sample_instance(sample);
    if (!instance) {
        DEBUG_MSG(bgm instance creation fail);
        al_destroy_sample(sample);
        return NULL;
    }

    al_attach_sample_instance_to_mixer(instance, al_get_default_mixer());
    al_set_sample_instance_playmode(instance, ALLEGRO_PLAYMODE_LOOP);
    al_play_sample_instance(instance);

    return instance;
}
