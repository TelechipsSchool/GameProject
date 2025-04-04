#include "cosmic_protector.h"

Rocket rocket = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0 };
Bullet bullets[MAX_BULLETS] = { 0 };
Asteroid asteroids[MAX_ASTEROIDS] = { 0 };  
Alien alien1 = { 0 };
Alien alien2 = { 0 };
Alien alien3 = { 0 };

bool key_state[ALLEGRO_KEY_MAX] = { false };
bool trail_flag = false; 

ALLEGRO_TIMER* timer = 0;
int warning_sign_timer = WARNING_TIME;
int blood_timer = BLOOD_TIME;
int blood_flag = 0;
float limit_time = 0;
int life_flag = 0;
int boss_timer = 20;
bool alien3_direction_flag = true;
bool boss_die_flag = false;

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

// 화면 흔들림 효과
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

/* 추가 */
int life = 7;
int heart_x_pos;
int Score;
char* Username;

void game2(char* username, int score, int high_score) {
    life = 7;
    Username = username;
    Score = score;
    alien_start_time = 0;
    alien_start_time2 = 0;
    alien_start_time3 = 0;
    game_start_time = time(NULL);

    
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 60);
    ALLEGRO_EVENT event;

    ALLEGRO_SAMPLE* bgm = al_load_sample("sfx/game2_bg.ogg");
    ALLEGRO_SAMPLE* bgm2 = al_load_sample("sfx/game2_bg2.ogg");
    al_play_sample(bgm, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

    loadBitmap();

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    bool running = true;
    bool redraw = true;
    al_start_timer(timer);
    rocket.active = true;
    rocket.invisible_timer = 0;
    alien1.active = false;
    alien2.active = false;
    alien1.counts = 0;
    alien1.angle = 0.0;
    alien3.hits = 0;

    bool play_once = true;

    while (running) {
        al_wait_for_event(event_queue, &event);

        // switch 문으로 바꿔야 할수도
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            key_state[event.keyboard.keycode] = true;
            // 총알 나가는 건 스페이스 계속 누르고 있어도 한 번만 나가도록 설정.
            if (key_state[ALLEGRO_KEY_SPACE]) fire_bullet();
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            key_state[event.keyboard.keycode] = false;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            // 로켓 앞으로 전진
            if (key_state[ALLEGRO_KEY_UP]) {
                update_rocket();
            }// 기울기 10% 이동. y값 증가 0.1
            if (key_state[ALLEGRO_KEY_LEFT]) {
                rocket.angle -= getRadian(5.7);
            }// 기울기 10% 이동. y값 감소 0.1
            if (key_state[ALLEGRO_KEY_RIGHT]) {
                rocket.angle += getRadian(5.7);
            }

            update_bullets();
            check_boss_collisions();
            if (!alien3.active) {
                update_asteroids();
                check_collisions();
                spawn_asteroids();
                check_die();
                alien_create();
                alien_update();
                alien_bullets();
                alien_update_bullets();
                check_alien_collisions();
                check_die_because_alien();
                check_die_because_alien_bullet();
                alien2_create();
                alien2_die();
                check_die_because_alien2();
                if (alien_start_time - game_start_time > 35 && play_once) {
                    al_stop_samples();
                    story3();
                    al_play_sample(bgm2, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                    play_once = false;                                      
                }
            }
            boss_create();
            boss_update();
            boss_update_bullets();
            boss_bullets();
            check_die_because_boss_bullet();
            check_die_because_boss();
            
            //check_life();
            if (bullet_interval > 0) bullet_interval--;
            if (boss_bullet_interval > 0) boss_bullet_interval--;
            redraw = true;

            if (life <= 0) {
                // 패배
                al_stop_samples();
                save_score(username, Score);      
                story5();
                running = false;                
                menu();
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_scene();
            // 하트 표시
            heart_x_pos = 1300;            
            for (int i = life; i > 0; --i) {
                al_draw_bitmap(heart, heart_x_pos, 40, 0);
                heart_x_pos += 40;
            }
            for (int i = 7 - life; i > 0; --i) {
                al_draw_bitmap(empty_heart, heart_x_pos, 40, 0);
                heart_x_pos += 40;
            }
            // 점수 표시
            al_draw_filled_rectangle(40, 750, 200, 850, al_map_rgb(100, 100, 100));
            al_draw_filled_rectangle(50, 760, 190, 840, al_map_rgb(50, 50, 50));
            al_draw_text(score_text_font, al_map_rgb(255, 255, 255), 120, 850, ALLEGRO_ALIGN_CENTER, "SCORE");
            al_draw_textf(score_best_font, al_map_rgb(255, 255, 255), 120, 815, ALLEGRO_ALIGN_CENTER, "BEST: %d", (Score > high_score) ? Score : high_score);
            al_draw_textf(score_font, al_map_rgb(255, 255, 255), 120, 770, ALLEGRO_ALIGN_CENTER, "%d", Score);
            al_flip_display();
        }
        trail_flag = false;
    }

    destroyBitmap();
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}


void loadBitmap() {
    background = al_load_bitmap("gfx/background5.png");
    ship = al_load_bitmap("gfx/ship.png");
    explosion_large = al_load_bitmap("gfx/large_explosion_2.png");
    explosion_small = al_load_bitmap("gfx/large_explosion_0.png");
    bulletIMG = al_load_bitmap("gfx/large_bullet.png");
    asteroidIMG_large = al_load_bitmap("gfx/large_asteroid.png");
    asteroidIMG_small = al_load_bitmap("gfx/medium_asteroid.png");
    invisible_ship = al_load_bitmap("gfx/ship_invisible7.png");
    trail = al_load_bitmap("gfx/trail.png");
    logo = al_load_bitmap("gfx/logo.png");
    alien1_withUFO = load_bitmap_resized("gfx/alien2.png", 120, 120);
    warning = load_bitmap_resized("gfx/warning3.png", 310, 100);
    //warning = load_bitmap_resized("gfx/warning2.png", 300, 100);
    //warning = load_bitmap_resized("gfx/alien2.png", 130, 130);
    alien_bullet = load_bitmap_resized("gfx/small_bullet.png", 10, 10);
    alien1_die = load_bitmap_resized("gfx/blood1.png", 250, 250);
    blood2 = load_bitmap_resized("gfx/blood5.png", 200, 200);
    //alien2_withoutUF0 = load_bitmap_resized("gfx/alien6_withoutUFO.png", 170, 170);
    //alien2_withoutUF0 = load_bitmap_resized("gfx/alien_snail.png", 100, 100);
    alien2_withoutUF0 = load_bitmap_resized("gfx/alien_withoutUFO2.png", 130, 130);
    //alien2IMG = load_bitmap_resized("gfx/alien_effect6.jpg", 270, 225);
    alien2IMG = load_bitmap_resized("gfx/alien6_withoutUFO.png", 130, 130);
    alien3IMG = load_bitmap_resized("gfx/boss7.png", 320, 200);
    warning_alien3 = load_bitmap_resized("gfx/warning_alien3_2.png", 620, 100);
    boss_explosion = load_bitmap_resized("gfx/large_explosion_1.png", 200, 200);
    boss_bullet = load_bitmap_resized("gfx/bullet3.png", 20, 20);

    ALLEGRO_BITMAP* arr[] = { background, ship, explosion_large, explosion_small, bulletIMG,
        asteroidIMG_large, asteroidIMG_small, invisible_ship, trail, logo, alien1_withUFO, warning, alien_bullet, alien1_die, blood2, alien2_withoutUF0, alien2IMG, alien3IMG, warning_alien3 , boss_explosion };
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
        if (arr[i] == NULL) DEBUG_MSG("%s bitmap is NULL\n", arr[i]);
    /* 추가 */
    heart = load_bitmap_resized("images/heart.png", 40, 40);
    empty_heart = load_bitmap_resized("images/empty_heart.png", 40, 40);
    /* 추가 끝 */
}

void destroyBitmap() {
    destroy_bitmap_stage3();
    al_destroy_bitmap(background);
    al_destroy_bitmap(ship);
    al_destroy_bitmap(explosion_large);
    al_destroy_bitmap(explosion_small);
    al_destroy_bitmap(invisible_ship);
    al_destroy_bitmap(heart);
    al_destroy_bitmap(empty_heart);
}

void destroy_bitmap_stage3() {
    al_destroy_bitmap(asteroidIMG_large);
    al_destroy_bitmap(asteroidIMG_small);
    al_destroy_bitmap(logo);
    al_destroy_bitmap(asteroidIMG_small);
    al_destroy_bitmap(alien1_withUFO);
    al_destroy_bitmap(warning);
    al_destroy_bitmap(alien_bullet);
    al_destroy_bitmap(alien1_die);
    al_destroy_bitmap(blood2);
    al_destroy_bitmap(alien2_withoutUF0);
    al_destroy_bitmap(alien2IMG);
}


double getRadian(int num) {
    return num * (PI / 180);
}

// 로켓 앞으로 이동
void update_rocket() {
    trail_flag = true;
    if (rocket.x < 0 || rocket.x > SCREEN_WIDTH || rocket.y < 0 || rocket.y > SCREEN_HEIGHT) {
        if (rocket.x <= 0) rocket.x += SCREEN_WIDTH;
        if (rocket.x >= SCREEN_WIDTH) rocket.x = 0;
        if (rocket.y <= 0) rocket.y += SCREEN_HEIGHT;
        if (rocket.y >= SCREEN_HEIGHT) rocket.y = 0;

    }
    else {
        rocket.x -= sin(-rocket.angle) * 3;  // 1.5
        rocket.y -= cos(-rocket.angle) * 3;
    }
}

// 총알 발사 (장전)
void fire_bullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = rocket.x + cos(rocket.angle) * ROCKET_SIZE;
            bullets[i].y = rocket.y + sin(rocket.angle) * ROCKET_SIZE;
            bullets[i].dx = cos(-rocket.angle) * BULLET_SPEED;
            bullets[i].dy = sin(-rocket.angle) * BULLET_SPEED;
            bullets[i].active = true;
            bullets[i].angle = rocket.angle;
            break;
        }
    }
    play_music("audio/shot.ogg");
}
// 총알 전진
void update_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].x -= bullets[i].dy;
            bullets[i].y -= bullets[i].dx;
            if (bullets[i].x < 0 || bullets[i].x > SCREEN_WIDTH || bullets[i].y < 0 || bullets[i].y > SCREEN_HEIGHT) {
                bullets[i].active = false;
            }
        }
    }
}

// alien 1 외계인 생성
void alien_create() {
    // 진짜 난수 생성을 위한 시드 변경 추가 - 추가했더니 다른 로직도 다 틀어짐. (일단 보류)
    //srand((unsigned)time(NULL));
    // 게임 시작 후 일정 시간 지나야 1단계 외계인이 출몰하도록 설정
    alien_start_time = time(NULL);
    double tt = alien_start_time - game_start_time;
    if (!alien1.active && rand() % 100 >= 95 && tt > 35) {  
        alien1.x = 0;
        alien1.y = rand() % SCREEN_HEIGHT;
        alien1.dx = 2;
        alien1.active = true;
        alien1.counts++;
        alien1.hits = 0;
        screen_shake_timer = 30;
        screen_shake_timer = 0;
    }
}
// alien1 외계인 오른쪽 이동
void alien_update() {
    if (alien1.active) {
        alien1.x += alien1.dx;

        if (alien1.x >= SCREEN_WIDTH) {
            alien1.active = false;
            screen_shake_timer = 0;
        }
    }
}

// alien 1 외계인 총알 생성
void alien_bullets() {
    float target_x, target_y;
    float dir_x, dir_y;
    float length;

    if (alien1.active && bullet_interval <= 0) {
        for (int i = 0; i < MAX_BULLETS_ALEIN; i++) {
            if (!alien1.bullets[i].active) {
                // 화면 중앙 좌표
                target_x = rocket.x;
                target_y = rocket.y;

                // 벡터 계산
                dir_x = alien1.x - target_x;
                dir_y = alien1.y - target_y;
                length = sqrt(dir_x * dir_x + dir_y * dir_y);

                // 정규화
                dir_x /= length;
                dir_y /= length;

                // 총알 초기 설정
                alien1.bullets[i].x = alien1.x + 50;
                alien1.bullets[i].y = alien1.y + 40;
                alien1.bullets[i].dx = dir_x * BULLET_SPEED_ALEIN;
                alien1.bullets[i].dy = dir_y * BULLET_SPEED_ALEIN;

                alien1.bullets[i].active = true;
                bullet_interval = BULLET_INTERVAL_ALEIN;

                alien1.bullets[i].active = true;
                break;  // 하나만 발사
            }
        }
    }
}

// alien1 외계인 총알 이동
void alien_update_bullets() {
    for (int i = 0; i < MAX_BULLETS_ALEIN; i++) {
        if (alien1.bullets[i].active) {
            alien1.bullets[i].x -= alien1.bullets[i].dx;
            alien1.bullets[i].y -= alien1.bullets[i].dy;

            // 화면 밖으로 나가면 비활성화
            if (alien1.bullets[i].x < 0 || alien1.bullets[i].x > SCREEN_WIDTH ||
                alien1.bullets[i].y < 0 || alien1.bullets[i].y > SCREEN_HEIGHT) {
                alien1.bullets[i].active = false;
            }
        }
    }
}

// alien 2 외계인 생성
void alien2_create() {
    alien_start_time2 = time(NULL);
    double tt = alien_start_time2 - game_start_time;
    if (!alien2.active && rand() % 100 >= 95 && tt > 70) {  
        alien2.x = rand() % SCREEN_WIDTH;
        alien2.y = rand() % SCREEN_HEIGHT;
        alien2.active = true;
        alien2.counts++;
        alien2.hits = 0;
        screen_shake_timer = 30;
        screen_shake_timer = 0;
        limit_time = rand() % 700;
    }
}
// 일정시간 지나면 alien2 사라짐
void alien2_die() {
    if (alien2.active && limit_time > 0) {
        limit_time--;
        if ((int)limit_time % 10 >= 6) {
            alien2.x += 15;
            alien2.y += 3;
        }
        else if ((int)limit_time % 10 < 3) {
            alien2.x -= 15;
            alien2.y += 1;
        }
    }
    if (limit_time <= 0) alien2.active = false;
}


// 마지막 최종 보스 출현
void boss_create() {
    alien_start_time3 = time(NULL);
    double tt = alien_start_time3 - game_start_time;
    if (!alien3.active && tt > 120 && !alien3.hits) {   
        alien3.x = SCREEN_WIDTH / 2 - 150;
        alien3.y = 0;
        alien3.dx = 3;  // 속도 조절
        alien3.active = true;
        alien3.counts++;
        alien3.hits = 0;
        boss_timer = BOSS_TIME;
        screen_shake_timer = 50;
    }
}
// 보스 이동 (좌우)
void boss_update() {
    if (alien3.active) {
        alien3.counts++;

        if (alien3_direction_flag) {
            alien3.x += alien3.dx;
        }
        else {
            alien3.x -= alien3.dx;
        }
        if (alien3.x <= 100) alien3_direction_flag = true;
        else if (alien3.x > SCREEN_WIDTH - 450) alien3_direction_flag = false;
    }
}

// 총알과 외계인3=보스 충돌 감지 (15번 맞으면 보스 사라지는 것으로 설정)
void check_boss_collisions() {
    float dx, dy;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            if (alien3.active) {
                dx = bullets[i].x - alien3.x;
                dy = bullets[i].y - alien3.y;
                if (sqrt(dx * dx + dy * dy) < 100) {
                    bullets[i].active = false;
                    alien3.hits++;
                    if (alien3.hits >= BOSS_HITS) {
                        alien3.active = false;
                        screen_shake_timer += 80;
                        al_stop_samples();
                        play_music_effect("sfx/boss_die.ogg"); //외계인1 죽이는 소리 -> 보스 죽는 소리로 변경 필요 [원준]
                        // 여기에 이어서 바로 게임 승리 음향 추가하고
                        // 게임 종료 스토리로 넘어가면 될 듯
                        // 패배
                    }
                }
            }
        }
    }
    if (boss_timer < 10) {
        save_score(Username, Score);
        story4(Username);
        menu();
    }
}


// 보스 총알 생성
void boss_bullets() {
    float start_angle;
    float angle_step;
    float angle;

    if (alien3.active && boss_bullet_interval <= 0) {
        start_angle = alien3.angle - PI / 2;
        angle_step = PI / (BULLETS_BOSS_STEP - 1);  // 180도를 8등분

        for (int i = 0; i < MAX_BULLETS_BOSS; i++) {
            if (!alien3.bullets[i].active) {
                angle = start_angle + (i * angle_step);  // -90 ~ 90도 범위

                // 총알 초기 설정
                alien3.bullets[i].x = alien3.x + 180;
                alien3.bullets[i].y = alien3.y + 150;
                alien3.bullets[i].angle = angle;
                // 총알 방향
                alien3.bullets[i].dx = cos(angle) * BULLET_SPEED_ALEIN;
                alien3.bullets[i].dy = sin(angle) * BULLET_SPEED_ALEIN;

                alien3.bullets[i].active = true;
                alien3.bullets[i].hit_with_rocket = 0; 
            }
        }
        boss_bullet_interval = BULLET_INTERVAL_BOSS;
    }

}

// 보스 총알 이동
void boss_update_bullets() {
    for (int i = 0; i < MAX_BULLETS_ALEIN; i++) {
        if (alien3.bullets[i].active) {
            alien3.bullets[i].x -= alien3.bullets[i].dx;
            alien3.bullets[i].y -= alien3.bullets[i].dy;

            // 화면 밖으로 나가면 비활성화
            if (alien3.bullets[i].x < 0 || alien3.bullets[i].x > SCREEN_WIDTH ||
                alien3.bullets[i].y < 0 || alien3.bullets[i].y > SCREEN_HEIGHT) {
                alien3.bullets[i].active = false;
            }
        }
    }
}

// 로켓이 보스 총알에 맞아서 die
void check_die_because_boss_bullet() {
    float dx, dy;
    for (int i = 0; i < MAX_BULLETS_BOSS; i++) {
        if (alien3.bullets[i].active) {
            dx = rocket.x - alien3.bullets[i].x;
            dy = rocket.y - alien3.bullets[i].y;
            if (sqrt(dx * dx + dy * dy) < 29 && rocket.active) {

                rocket.active = false;
                rocket.invisible_timer = ROCKET_INVISIBLE_TIME;
                screen_shake_timer = 45;

                //life--;
                alien3.bullets[i].hit_with_rocket++;
                check_life(&(alien3.bullets[i]), 2);
            }
        }
        if (rocket.invisible_timer > 0) {
            rocket.invisible_timer--;
            if (rocket.invisible_timer == 0) rocket.active = true;
        }
    }
}

// 로켓과 보스 충돌 감지
void check_die_because_boss() {
    float dx, dy;
    if (alien3.active) {
        dx = rocket.x - alien3.x;
        dy = rocket.y - alien3.y;
        if (sqrt(dx * dx + dy * dy) < 29 && rocket.active) {

            rocket.active = false;
            rocket.invisible_timer = ROCKET_INVISIBLE_TIME;
            screen_shake_timer = 45;

            //life--;
            alien3.hit_with_rocket++;
            check_life(&alien3, 3);
        }
    }
    if (rocket.invisible_timer > 0) {
        rocket.invisible_timer--;
        if (rocket.invisible_timer == 0) rocket.active = true;
    }
}



// 소행성 랜덤 생성
void spawn_asteroids() {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active && (rand() % 100 >= 20)) {
            asteroids[i].x = rand() % SCREEN_WIDTH;
            asteroids[i].y = rand() % SCREEN_HEIGHT;
            do {
                asteroids[i].dx = (rand() % MAX_ASTEROIDS - 2) * 2;
                asteroids[i].dy = (rand() % MAX_ASTEROIDS - 2) * 2;
            } while (!asteroids[i].dx && !asteroids[i].dy);

            asteroids[i].hits = 0;
            asteroids[i].active = true;
        }
    }
}
// 소행성이 화면을 벗어나면 다시 생성
void update_asteroids() {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            asteroids[i].x += asteroids[i].dx * 0.5;
            asteroids[i].y += asteroids[i].dy * 0.5;
            if (asteroids[i].x < 0 || asteroids[i].x > SCREEN_WIDTH || asteroids[i].y < 0 || asteroids[i].y > SCREEN_HEIGHT) {
                asteroids[i].active = false;
            }
        }
    }
}
// 총알과 운석 충돌 감지 (5번 맞으면 운석 사라지는 것으로 설정)
void check_collisions() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            for (int j = 0; j < MAX_ASTEROIDS; j++) {
                if (asteroids[j].active) {
                    float dx = bullets[i].x - asteroids[j].x;
                    float dy = bullets[i].y - asteroids[j].y;
                    if (sqrt(dx * dx + dy * dy) < 40) {
                        bullets[i].active = false;
                        asteroids[j].hits++;
                        if (asteroids[j].hits >= ASTEROID_HITS) {
                            asteroids[j].active = false;
                            Score += 50;
                            play_music_effect("sfx/big_explosion.ogg");  //운석파괴 소리
                        }
                    }
                }
            }
        }
    }
}


// 총알과 외계인 충돌 감지 (10번 맞으면 외계인 사라지는 것으로 설정)
void check_alien_collisions() {
    float dx, dy;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            if (alien1.active) {
                dx = bullets[i].x - alien1.x;
                dy = bullets[i].y - alien1.y;
                if (sqrt(dx * dx + dy * dy) < 100) {
                    bullets[i].active = false;
                    alien1.hits++;
                    if (alien1.hits >= ALIEN_HITS) {
                        alien1.active = false;
                        Score += 100;
                        play_music_effect("sfx/powerup.ogg"); //외계인1 죽이는 소리
                    }
                }
            }
        }
    }
}


// 로켓과 소행성 감지 
void check_die() {
    float dx, dy;
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            dx = rocket.x - asteroids[i].x;
            dy = rocket.y - asteroids[i].y;
            if (sqrt(dx * dx + dy * dy) < 30 && rocket.active) {
                play_music_effect("sfx/collision.ogg");  // 로켓이랑 행성 충돌
                rocket.active = false;
                rocket.invisible_timer = ROCKET_INVISIBLE_TIME;
                screen_shake_timer = 45;
                //life--;
                asteroids[i].hit_with_rocket++;
                check_life(&asteroids[i], 1);
            }
        }
        if (rocket.invisible_timer > 0) {
            rocket.invisible_timer--;
            if (rocket.invisible_timer == 0) rocket.active = true;
        }
    }
}
// 로켓 생명 감소
void check_life(void* tmp, int flag) {
	switch (flag) {
	    case 1: {  // 로켓과 소행성 충돌 시 생명 가감.
		    if (((Asteroid*)tmp)->hit_with_rocket == 1 && !rocket.active) life--;
		    if (rocket.active) ((Asteroid*)tmp)->hit_with_rocket = 0;
	    }break;
	    case 2: {  // 로켓과 외계인 총알 충돌 시 생명 가감.
		    if (((Bullet*)tmp)->hit_with_rocket == 1 && !rocket.active) life--;
		    if (rocket.active) ((Bullet*)tmp)->hit_with_rocket = 0;
	    }break;
        case 3: {  // 로켓과 외계인 충돌 시 생명 가감.
            if (((Alien*)tmp)->hit_with_rocket == 1 && !rocket.active) life--;
            if (rocket.active) ((Alien*)tmp)->hit_with_rocket = 0;

        }break;
        default: break;
	}
    
}

// 로켓과 외계인 총알 감지
void check_die_because_alien_bullet() {
    float dx, dy;
    for (int i = 0; i < MAX_BULLETS_ALEIN; i++) {
        if (alien1.bullets[i].active) {
            dx = rocket.x - alien1.bullets[i].x;
            dy = rocket.y - alien1.bullets[i].y;
            if (sqrt(dx * dx + dy * dy) < 29 && rocket.active) {
                
                rocket.active = false;
                rocket.invisible_timer = ROCKET_INVISIBLE_TIME;
                screen_shake_timer = 45;

                //life--;
                alien1.bullets[i].hit_with_rocket++;
                check_life(&(alien1.bullets[i]), 2);
            }
        }
        if (rocket.invisible_timer > 0) {
            rocket.invisible_timer--;
            if (rocket.invisible_timer == 0) rocket.active = true;
        }
    }
}

// 로켓과 외계인 충돌 감지
void check_die_because_alien() {
    float dx, dy;
    if (alien1.active) {
        dx = rocket.x - alien1.x;
        dy = rocket.y - alien1.y;
        if (sqrt(dx * dx + dy * dy) < 29 && rocket.active) {
            
            rocket.active = false;
            rocket.invisible_timer = ROCKET_INVISIBLE_TIME;
            screen_shake_timer = 45;

            //life--;
            alien1.hit_with_rocket++;
            check_life(&alien1, 3);
        }
    }
    if (rocket.invisible_timer > 0) {
        rocket.invisible_timer--;
        if (rocket.invisible_timer == 0) rocket.active = true;
    }
}

// 로켓과 외계인2 충돌 감지
void check_die_because_alien2() {
    float dx, dy;
    if (alien2.active) {
        dx = rocket.x - alien2.x;
        dy = rocket.y - alien2.y;
        if (sqrt(dx * dx + dy * dy) < 34 && rocket.active) {
            rocket.active = false;
            rocket.invisible_timer = ROCKET_INVISIBLE_TIME;
            screen_shake_timer = 45;

            //life--;
            alien2.hit_with_rocket++;
            check_life(&alien2, 3);
        }
    }
    if (rocket.invisible_timer > 0) {
        rocket.invisible_timer--;
        if (rocket.invisible_timer == 0) rocket.active = true;
    }
}


// display
void draw_scene() {
    // 충돌 시 화면 흔들림 효과
    if (screen_shake_timer > 0) {
        // -10 ~ 10 범위의 랜덤값
        shake_offset_x = (rand() % 21) - 10;
        shake_offset_y = (rand() % 21) - 10;
        screen_shake_timer--;
    }
    else {
        shake_offset_x = 0;
        shake_offset_y = 0;
    }
    al_draw_bitmap(background, shake_offset_x, shake_offset_y, 0);

    // STAGE 2 - 외계인 출몰 및 STAGE2 시작 효과
    if ((alien1.active || alien2.active) && warning_sign_timer > 0 && boss_timer == BOSS_FLAG) {
        al_draw_bitmap(warning, shake_offset_x, shake_offset_y + 40, 0);
        warning_sign_timer--;
    }
    if (warning_sign_timer <= 0) warning_sign_timer = WARNING_TIME;


    // 로고 display
    if (init_logo_timer > 0 && boss_timer == BOSS_FLAG) {
        al_draw_bitmap(logo, SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 80, 0);
        init_logo_timer--;
    }
    else if(boss_timer == BOSS_FLAG) al_draw_bitmap(logo, shake_offset_x, shake_offset_y, 0);

    // 로켓 불꽃 - 앞으로 전진 시 display
    if (trail_flag) {
        al_draw_rotated_bitmap(trail, ROCKET_SIZE + 33, ROCKET_SIZE - 25, rocket.x + cos(rocket.angle) * 10, rocket.y + sin(rocket.angle) * 10, rocket.angle - getRadian(65), 0);
        al_draw_rotated_bitmap(trail, ROCKET_SIZE + 43, ROCKET_SIZE - 15, rocket.x + cos(rocket.angle) * 10, rocket.y + sin(rocket.angle) * 10, rocket.angle - getRadian(115), 0);
    }

    // 로켓 display
    if (!rocket.active || rocket.invisible_timer > 0) {
        al_draw_rotated_bitmap(invisible_ship, 10, 10, rocket.x, rocket.y, rocket.angle, 0);
    }
    else al_draw_rotated_bitmap(ship, 10, 10, rocket.x, rocket.y, rocket.angle, 0);

    // 총알, 소행성 display
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            al_draw_rotated_bitmap(bulletIMG, 10, 10, bullets[i].x + 0.1, bullets[i].y + 0.1, bullets[i].angle, 0);
        }
    }
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active && boss_timer == BOSS_FLAG) {
            if (!i % 3) al_draw_bitmap(asteroidIMG_large, asteroids[i].x, asteroids[i].y, 0);
            else al_draw_bitmap(asteroidIMG_small, asteroids[i].x, asteroids[i].y, 0);

        }
        else if (asteroids[i].hits >= ASTEROID_HITS) {
            if (!i % 2) al_draw_bitmap(explosion_large, asteroids[i].x - 20, asteroids[i].y - 20, 0);
            else al_draw_bitmap(explosion_small, asteroids[i].x - 20, asteroids[i].y - 20, 0);
        }
    }


    // alien, alien bullets display
    if (alien1.active && boss_timer == BOSS_FLAG) {
        al_draw_bitmap(alien1_withUFO, alien1.x, alien1.y, 0);
        for (int i = 0; i < MAX_BULLETS_ALEIN; i++) {
            if (alien1.bullets[i].active) al_draw_bitmap(alien_bullet, alien1.bullets[i].x + 0.1, alien1.bullets[i].y + 0.1, 0);
        }
    } // alien 죽으면 피 display
    if (alien1.hits >= ALIEN_HITS && blood_timer >= 0 && !alien1.active) {
        al_draw_bitmap(alien1_die, alien1.x - 20, alien1.y - 20, 0);
        blood_flag++;
        blood_timer--;
        if (blood_timer <= 0) {
            blood_timer = BLOOD_TIME;
        }
    }
    if (blood_flag > 0) {
        //if (blood_flag >= 1) al_draw_bitmap(blood2, 0, 0, 0);
    }

    // alien2 display
    if (alien2.active && boss_timer == BOSS_FLAG) {
        al_draw_bitmap(alien2_withoutUF0, alien2.x, alien2.y, 0);
    }

    // alien3 보스 display
    // 보스가 나타나면 화면에 모든 객체들은 사라짐(소행성, 외계인1,2)
    if (alien3.hits >= BOSS_HITS) {
        boss_timer--;
        al_draw_bitmap(boss_explosion, alien3.x, alien3.y, 0);
    }
    if (alien3.active) {
        // alien3 보스 총알
        for (int i = 0; i < MAX_BULLETS_BOSS; i++) {
            if (alien3.bullets[i].active) al_draw_bitmap(boss_bullet, alien3.bullets[i].x + 0.1, alien3.bullets[i].y + 0.1, 0);
        }
        al_draw_bitmap(alien3IMG, alien3.x, alien3.y, 0);
        if (alien3.counts <= 130) {
            al_draw_bitmap(warning_alien3, shake_offset_x + 500, shake_offset_y + 100, 0);   
            play_music("sfx/monsterrat");
        }
    }

}