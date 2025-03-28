#pragma once
#include "init.h"
#include "display.h"
#include "vector.h"

// 유도선 설정
float line_length = 0.8f;

// 행성 발사 위치
#define init_x 300
#define init_y SCREEN_H / 2

// 목표 행성 위치
#define center_x SCREEN_W - 400
#define center_y SCREEN_H / 2

// 물리 계수
double GRAVITY = 9.8f * 100;  // 중력
double RESTITUTION = 0.5f;      // 탄성 계수

// 최대 행성 개수
#define MAX_PLANET 100

typedef struct {
    Vector2 position;
    Vector2 velocity;
    bool isFlying;
    int type;
    int radius;
} Planet;

void start_game();
bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2);
// 핵심 중력 + 회전 보정 함수
void CalcGravity(Planet* rb, Vector2 center, float centerCoefficient, float deltaTime);