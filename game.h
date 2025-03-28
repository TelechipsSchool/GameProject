#pragma once
#include "init.h"
#include "display.h"
#include "vector.h"

// 유도선 설정
#define line_length 100

// 행성 발사 위치
#define init_x 300
#define init_y SCREEN_H / 2

// 목표 행성 위치
#define center_x SCREEN_W - 400
#define center_y SCREEN_H / 2

// 물리 계수
float gravity_strength  = 0.05;  // 중력
float restitution = 1;      // 탄성 계수

typedef struct {
    float x, y;
    float speed;
    float angle;
    int width;
    int height;
    ALLEGRO_BITMAP* image;
} Planet;

void start_game();
Planet create_planet(float x, float y, float speed, float angle, int width, int height, const char* image_path);
bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2);
// 핵심 중력 + 회전 보정 함수
void CalcGravity(Rigidbody2D* rb, Vector2 center, float gravityScale, float centerCoefficient, float deltaTime);