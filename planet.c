#include "planet.h"

int score = 0;

Planet* Create_Planet(Vector2 pos, Vector2 vel, int type) {
    Planet* p = (Planet*)calloc(1, sizeof(Planet));
    if (!p) {
        DEBUG_MSG(Create_Planet - 메모리 부족);
        return 0;
    }
    p->position = pos;
    p->velocity = vel;
    p->type = type;
    p->radius = get_radius(type);
    p->isFlying = false;
    p->wasInGravityZone = false;
    p->isLeavingGravityZone = false;
    p->rotation = 0.0f;
    p->angularVelocity = ((float)(rand() % 200) - 100) / 100.0f;
    return p;
}

void Destroy_Planet(Planet** planet_list, int* count, int index) {
    if (!planet_list[index]) return;//0이면 없는 상태.
    free(planet_list[index]);
    planet_list[index] = NULL;
    //뒤 행성들을 앞으로 당겨서 빈칸 없애기.
    for (int i = index; i < (*count) - 1; ++i) {
        planet_list[i] = planet_list[i + 1];
    }
    planet_list[*count - 1] = NULL;
    (*count)--;
}

int get_radius(int type) {
    switch (type) {
    case 1: return 15; break;
    case 2: return 20; break;
    case 3: return 30; break;
    case 4: return 45; break;
    case 5: return 60; break;
    case 6: return 80; break;
    case 7: return 150; break;
    }
}

void merge_planets(Planet* a, Planet* b) {
    play_music("audio/enter.ogg");
    a->type += 1;
    a->radius = get_radius(a->type);
    a->velocity = Vector2_Scale(Vector2_Add(a->velocity, b->velocity), 0.5f);

    b->isFlying = false;
    b->type = 0;
    b->velocity = (Vector2){ 0,0 };
}

void reset_planets() {
    for (int i = 0; i < planet_num; ++i) {
        Destroy_Planet(planet_list, &planet_num, i);
    }
    for (int i = 0; i < MAX_PLANET; ++i) {
        planet_list[i] = NULL;
    }
    planet_num = 0;
    score = 0;
}

void create_initial_planets(Vector2 shootStart, Vector2 waitPoint, int max_type) {
    planet_list[planet_num++] = Create_Planet(shootStart, (Vector2) { 0, 0 }, rand() % max_type + 1);
    planet_list[planet_num++] = Create_Planet(waitPoint, (Vector2) { 0, 0 }, rand() % max_type + 1);
}

int get_max_type_by_difficulty(int difficulty) {
    switch (difficulty) {
    case DIFFICULTY_EASY: printf("난이도: EASY\n"); return 4;
    case DIFFICULTY_MEDIUM: printf("난이도: NORMAL\n"); return 3;
    case DIFFICULTY_HARD: printf("난이도: HARD\n"); return 2;
    default: return 3;
    }
}
