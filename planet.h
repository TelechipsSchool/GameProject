#pragma once
#include "init.h"
#include "vector.h"
#include "setting.h"

// �ִ� �༺ ����
#define MAX_PLANET 100
#define PLANET_TYPES 7

typedef struct {
    Vector2 position;
    Vector2 velocity;
    bool isFlying;
    int type;
    int radius;
    bool wasInGravityZone;
    bool isLeavingGravityZone;
    float rotation;         // ȸ�� ����
    float angularVelocity;  // ���ӵ�
} Planet;

Planet* planet_list[MAX_PLANET] = { 0 };
int planet_num = 0;

extern int score;

int get_radius(int type);
Planet* Create_Planet(Vector2 pos, Vector2 vel, int type);
void Destroy_Planet(Planet** planet_list, int* count, int index);
void merge_planets(Planet* a, Planet* b);
void reset_planets();
void create_initial_planets(Vector2 shootStart, Vector2 waitPoint, int max_type);
int get_max_type_by_difficulty(int difficulty);