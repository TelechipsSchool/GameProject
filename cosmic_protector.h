#ifndef COSMIC_PROTECTOR_H
#define COSMIC_PROTECTOR_H

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
#define BOSS_FLAG 20
#define BOSS_TIME 60
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

// Bitmap
ALLEGRO_BITMAP* background = NULL;
ALLEGRO_BITMAP* ship = NULL;
ALLEGRO_BITMAP* explosion_large = NULL;
ALLEGRO_BITMAP* explosion_small = NULL;
ALLEGRO_BITMAP* bulletIMG = NULL;
ALLEGRO_BITMAP* asteroidIMG_large = NULL;
ALLEGRO_BITMAP* asteroidIMG_small = NULL;
ALLEGRO_BITMAP* invisible_ship = NULL;
ALLEGRO_BITMAP* trail = NULL;
ALLEGRO_BITMAP* logo = NULL;
ALLEGRO_BITMAP* alien1_withUFO = NULL;
ALLEGRO_BITMAP* warning = NULL;
ALLEGRO_BITMAP* alien_bullet = NULL;
ALLEGRO_BITMAP* alien1_die = NULL;
ALLEGRO_BITMAP* blood2 = NULL;
ALLEGRO_BITMAP* alien2IMG = NULL;
ALLEGRO_BITMAP* alien2_withoutUF0 = NULL;
ALLEGRO_BITMAP* alien3IMG = NULL;
ALLEGRO_BITMAP* warning_alien3 = NULL;
ALLEGRO_BITMAP* boss_explosion = NULL;
ALLEGRO_BITMAP* boss_bullet = NULL;
ALLEGRO_BITMAP* heart = NULL;
ALLEGRO_BITMAP* empty_heart = NULL;

// 전역변수
Rocket rocket = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0 };
Bullet bullets[MAX_BULLETS] = { 0 };
Asteroid asteroids[MAX_ASTEROIDS] = { 0 };

Alien alien1 = { 0 };
Alien alien2 = { 0 };
Alien alien3 = { 0 };

ALLEGRO_TIMER* timer = 0;
ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_SAMPLE* bgm;
ALLEGRO_SAMPLE* bgm2;
ALLEGRO_EVENT event;

bool key_state[ALLEGRO_KEY_MAX] = { false };
bool trail_flag = false;
bool alien3_direction_flag = true;
bool running = true;
bool redraw = true;
bool play_once = true;

int warning_sign_timer = WARNING_TIME;
int blood_timer = BLOOD_TIME;
int blood_flag = 0;
float limit_time = 0;
int life_flag = 0;
int boss_timer = 20;

int screen_shake_timer = 0;
float shake_offset_x = 0;
float shake_offset_y = 0;

float init_logo_timer = 600;
int bullet_interval = 0;
int boss_bullet_interval = 0;

double game_start_time;
double alien_start_time;
double alien_start_time2;
double alien_start_time3;
double tt;

int life = 7;
int heart_x_pos;
int Score;
char* Username;


// 함수 선언
void game2(char* username, int score, int high_score);
void loadBitmap();
void destroyBitmap();
void game2_init(char*, int);
void heart_display(int);
void game_over();
void normal();
void boss();
void clear();
void get_score(int);
void update_rocket();
void update_bullets();
void fire_bullet();
void draw_scene();
void spawn_asteroids();
void update_asteroids();
void check_collisions();
double getRadian(int num);
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
void boss_create();
void boss_update();
void check_boss_collisions();
void boss_update_bullets();
void boss_bullets();
void check_die_because_boss_bullet();
void check_die_because_boss();

#endif