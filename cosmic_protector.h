#pragma once
#include "init.h"
#include "display.h"
#include "vector.h"
#include "font.h"
#include "rank.h"
#include "story.h"

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
#define BULLET_SPEED_ALEIN 6
#define MAX_BULLETS 1000
#define MAX_BULLETS_ALEIN 500
#define MAX_BULLETS_BOSS 500
#define BULLETS_BOSS_STEP 8
#define MAX_ASTEROIDS 5
#define ASTEROID_HITS 5
#define BLOOD_TIME 70
#define WARNING_TIME 5
#define ALIEN_HITS 10
#define BOSS_HITS 15
#define ROCKET_HITS 1
#define ROCKET_INVISIBLE_TIME 2500
#define BULLET_INTERVAL_ALEIN 60
#define BULLET_INTERVAL_BOSS 60
#define PI 3.1415926535897
#define DEBUG_MSG(msg) printf("[DEBUG]: %s\n", msg)

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
    float angle;
    int hit_with_rocket;
} Bullet;

typedef struct {
    float x, y, dx, dy;
    int hits;
    bool active;
    int hit_with_rocket;
} Asteroid;

typedef struct {
    float x, y, dx, dy;
    int hits;
    float angle;
    bool active;
    Bullet bullets[MAX_BULLETS_ALEIN];
    int counts;
    int hit_with_rocket;
} Alien;


void game2(char* username, int score, int high_score);
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
void check_alien_collisions();
void check_die_because_alien();
void check_die_because_alien_bullet();
void alien2_create();
void alien2_die();
void check_die_because_alien2();
void check_life(void*, int);
void destroy_bitmap_stage3();
void boss_create();
void boss_update();
void check_boss_collisions();
void boss_update_bullets();
void boss_bullets();
void check_die_because_boss_bullet();
void check_die_because_boss();