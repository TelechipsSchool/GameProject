#include "planet.h"

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