#pragma once
#include "init.h"
#include "display.h"
#include "vector.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//#define SCREEN_WIDTH 800
//#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define ROCKET_SIZE 20
#define BULLET_SPEED 5
#define MAX_BULLETS 1000
#define MAX_ASTEROIDS 5
#define ASTEROID_HITS 5
#define PI 3.1415926535897
#define DEBUG_MSG(msg) printf("[DEBUG]: %s\n", msg)

//typedef struct {
//    float x, y;
//    float dx, dy;
//}Vector2;

typedef struct {
    float x, y, dx, dy;
    float angle;
} Rocket;

typedef struct {
    Vector2 vec;
    float x, y, dx, dy;
    bool active;
} Bullet;

typedef struct {
    float x, y, dx, dy;
    int hits;
    bool active;
} Asteroid;


void update_rocket();
void update_bullets();
void fire_bullet();
void draw_scene();
void spawn_asteroids();
void update_asteroids();
void check_collisions();
double getRadian(int num);
void loadBitmap();
void destroyBitmap();