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
#define BULLET_SPEED_ALLEIN 2
#define MAX_BULLETS 1000
#define MAX_BULLETS_ALLEIN 500
#define MAX_ASTEROIDS 5
#define ASTEROID_HITS 5
#define ROCKET_HITS 1
#define ROCKET_INVISIBLE_TIME 2500
#define PI 3.1415926535897
#define DEBUG_MSG(msg) printf("[DEBUG]: %s\n", msg)

//typedef struct {
//    float x, y;
//    float dx, dy;
//}Vector2;

typedef struct {
    float x, y, dx, dy;
    float angle;
    float invisible_timer;
    int hits;
    bool active;
} Rocket;

typedef struct {
    float x, y, dx, dy;
    bool active;
} Bullet;

typedef struct {
    float x, y, dx, dy;
    int hits;
    bool active;
} Asteroid;

typedef struct {
    float x, y, dx, dy;
    int hits;
    float angle;
    bool active;
    Bullet bullets[MAX_BULLETS_ALLEIN];
    int counts;
} Alien;


void game2(int score);
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
void check_die();
void alien_create();
void alien_update();
void alien_bullets();
void alien_update_bullets();