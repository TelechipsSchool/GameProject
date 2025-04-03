#pragma once
#include "init.h"
#include "Vector.h"

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

Planet* Create_Planet(Vector2 pos, Vector2 vel, int type);
void Destroy_Planet(Planet** planet_list, int* count, int index);
void merge_planets(Planet* a, Planet* b);