#pragma once
#include "init.h"
#include "display.h"

// 유도선 설정
#define line_length 100

// 행성 발사 위치
#define init_x 100
#define init_y SCREEN_H / 2

// 목표 행성 위치
#define goal_x SCREEN_W - 100
#define goal_y SCREEN_H / 2

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