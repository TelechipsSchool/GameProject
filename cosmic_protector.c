#include "cosmic_protector.h"

Rocket rocket = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0 };
Bullet bullets[MAX_BULLETS] = { 0 };
Asteroid asteroids[MAX_ASTEROIDS] = { 0 };  // ť�� �ٲ㼭 �޸� ���� ���� ���ֱ�
Alien alien1 = { 0 };
Alien alien2 = { 0 };


bool key_state[ALLEGRO_KEY_MAX] = { false };
bool trail_flag = false; 

ALLEGRO_TIMER* timer = 0;
int blood_timer = BLOOD_TIME;
int blood_flag = 0;
float limit_time = 0;

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

ALLEGRO_BITMAP* heart = NULL;
ALLEGRO_BITMAP* empty_heart = NULL;

// ȭ�� ��鸲 ȿ��
int screen_shake_timer = 0;
float shake_offset_x = 0;
float shake_offset_y = 0;

float init_logo_timer = 600;
int bullet_interval = 0;

double game_start_time;
double alien_start_time;
double alien_start_time2;
double tt;

/* �߰� */
int life = 7;
int heart_x_pos;

void game2(char* username, int score, int high_score) {
    game_start_time = time(NULL);

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 60);
    ALLEGRO_EVENT event;

    loadBitmap();

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
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

    while (running) {
        al_wait_for_event(event_queue, &event);

        // switch ������ �ٲ�� �Ҽ���
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            key_state[event.keyboard.keycode] = true;
            // �Ѿ� ������ �� �����̽� ��� ������ �־ �� ���� �������� ����.
            if (key_state[ALLEGRO_KEY_SPACE]) fire_bullet();
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            key_state[event.keyboard.keycode] = false;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            // ���� ������ ����
            if (key_state[ALLEGRO_KEY_UP]) {
                update_rocket();
            }// ���� 10% �̵�. y�� ���� 0.1
            if (key_state[ALLEGRO_KEY_LEFT]) {
                rocket.angle -= getRadian(5.7);
            }// ���� 10% �̵�. y�� ���� 0.1
            if (key_state[ALLEGRO_KEY_RIGHT]) {
                rocket.angle += getRadian(5.7);
            }
            update_bullets();
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
            if (bullet_interval > 0) bullet_interval--;
            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_scene();
            // ��Ʈ ǥ��
            heart_x_pos = 1300;
            if (heart <= 0) {
                // �й�
            }
            for (int i = life; i > 0; --i) {
                al_draw_bitmap(heart, heart_x_pos, 40, 0);
                heart_x_pos += 40;
            }
            for (int i = 7 - life; i > 0; --i) {
                al_draw_bitmap(empty_heart, heart_x_pos, 40, 0);
                heart_x_pos += 40;
            }
            // ���� ǥ��
            al_draw_filled_rectangle(40, 750, 200, 850, al_map_rgb(100, 100, 100));
            al_draw_filled_rectangle(50, 760, 190, 840, al_map_rgb(50, 50, 50));
            al_draw_text(score_text_font, al_map_rgb(255, 255, 255), 120, 850, ALLEGRO_ALIGN_CENTER, "SCORE");
            al_draw_textf(score_best_font, al_map_rgb(255, 255, 255), 120, 815, ALLEGRO_ALIGN_CENTER, "BEST: %d", (score > high_score) ? score : high_score);
            al_draw_textf(score_font, al_map_rgb(255, 255, 255), 120, 770, ALLEGRO_ALIGN_CENTER, "%d", score);
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
    warning = load_bitmap_resized("gfx/warning3.png", 300, 80);
    //warning = load_bitmap_resized("gfx/warning2.png", 300, 100);

    //warning = load_bitmap_resized("gfx/alien2.png", 130, 130);
    alien_bullet = load_bitmap_resized("gfx/small_bullet.png", 10, 10);
    alien1_die = load_bitmap_resized("gfx/blood1.png", 250, 250);
    blood2 = load_bitmap_resized("gfx/blood5.png", 200, 200);
    //alien2_withoutUF0 = load_bitmap_resized("gfx/alien6_withoutUFO.png", 170, 170);
    alien2_withoutUF0 = load_bitmap_resized("gfx/alien_snail.png", 100, 100);
    //alien2IMG = load_bitmap_resized("gfx/alien_effect6.jpg", 270, 225);
    alien2IMG = load_bitmap_resized("gfx/alien6_withoutUFO.png", 130, 130);

    ALLEGRO_BITMAP* arr[] = { background, ship, explosion_large, explosion_small, bulletIMG,
        asteroidIMG_large, asteroidIMG_small, invisible_ship, trail, logo, alien1_withUFO, warning, alien_bullet, alien1_die, blood2, alien2_withoutUF0, alien2IMG };
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
        if (arr[i] == NULL) DEBUG_MSG("%s bitmap is NULL\n", arr[i]);
    /* �߰� */
    heart = load_bitmap_resized("images/heart.png", 40, 40);
    empty_heart = load_bitmap_resized("images/empty_heart.png", 40, 40);
    /* �߰� �� */
}

void destroyBitmap() {
    al_destroy_bitmap(background);
    al_destroy_bitmap(ship);
    al_destroy_bitmap(explosion_large);
    al_destroy_bitmap(explosion_small);
    al_destroy_bitmap(asteroidIMG_large);
    al_destroy_bitmap(asteroidIMG_small);
    al_destroy_bitmap(invisible_ship);
    al_destroy_bitmap(logo);
    al_destroy_bitmap(alien1_withUFO);
    al_destroy_bitmap(warning);
    al_destroy_bitmap(alien_bullet);
    al_destroy_bitmap(alien1_die);
    al_destroy_bitmap(blood2);
    al_destroy_bitmap(alien2_withoutUF0);
    al_destroy_bitmap(alien2IMG);

    al_destroy_bitmap(heart);
    al_destroy_bitmap(empty_heart);
}

double getRadian(int num) {
    return num * (PI / 180);
}

// ���� ������ �̵�
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

// �Ѿ� �߻� (����)
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
}
// �Ѿ� ����
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

// alien 1 �ܰ��� ����
void alien_create() {
    // ��¥ ���� ������ ���� �õ� ���� �߰� - �߰��ߴ��� �ٸ� ������ �� Ʋ����. (�ϴ� ����)
    //srand((unsigned)time(NULL));
    // ���� ���� �� ���� �ð� ������ 1�ܰ� �ܰ����� ����ϵ��� ����
    alien_start_time = time(NULL);
    double tt = alien_start_time - game_start_time;
    if (!alien1.active && rand() % 100 >= 95 && tt > 0) {   //  tt> 35 ������ tt > 35�� �س��� ��. ���� ������ϴ��� 0�� �ص�
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
// alien1 �ܰ��� ������ �̵�
void alien_update() {
    if (alien1.active) {
        alien1.x += alien1.dx;

        //alien1.angle -= getRadian(0.7); // �̰͵� ȭ�� �߽����� ����� ���� �޾Ƽ� ���� ��������� �ҵ�.
        if (alien1.x >= SCREEN_WIDTH) {
            alien1.active = false;
            screen_shake_timer = 0;
        }
    }
}

// alien 1 �ܰ��� �Ѿ� ����
void alien_bullets() {
    float target_x, target_y;
    float dir_x, dir_y;
    float length;

    if (alien1.active && bullet_interval <= 0) {
        for (int i = 0; i < MAX_BULLETS_ALEIN; i++) {
            if (!alien1.bullets[i].active) {
                // ȭ�� �߾� ��ǥ
                target_x = rocket.x;
                target_y = rocket.y;

                // ���� ���
                dir_x = alien1.x - target_x;
                dir_y = alien1.y - target_y;
                length = sqrt(dir_x * dir_x + dir_y * dir_y);

                // ����ȭ
                dir_x /= length;
                dir_y /= length;

                // �Ѿ� �ʱ� ����
                alien1.bullets[i].x = alien1.x + 50;
                alien1.bullets[i].y = alien1.y + 40;
                alien1.bullets[i].dx = dir_x * BULLET_SPEED_ALEIN;
                alien1.bullets[i].dy = dir_y * BULLET_SPEED_ALEIN;

                alien1.bullets[i].active = true;
                bullet_interval = BULLET_INTERVAL_ALEIN;

                alien1.bullets[i].active = true;
                break;  // �ϳ��� �߻�
            }
        }
    }

}

// alien1 �ܰ��� �Ѿ� �̵�
void alien_update_bullets() {
    for (int i = 0; i < MAX_BULLETS_ALEIN; i++) {
        if (alien1.bullets[i].active) {
            alien1.bullets[i].x -= alien1.bullets[i].dx;
            alien1.bullets[i].y -= alien1.bullets[i].dy;

            // ȭ�� ������ ������ ��Ȱ��ȭ
            if (alien1.bullets[i].x < 0 || alien1.bullets[i].x > SCREEN_WIDTH ||
                alien1.bullets[i].y < 0 || alien1.bullets[i].y > SCREEN_HEIGHT) {
                alien1.bullets[i].active = false;
            }
        }
    }
}


// alien 2 �ܰ��� ����
void alien2_create() {
    alien_start_time2 = time(NULL);
    double tt = alien_start_time2 - game_start_time;
    if (!alien2.active && rand() % 100 >= 95 && tt > 0) {   //  tt> 70? ������ tt > 70�� �س��� ��. ���� ������ϴ��� 0�� �ص�
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
// �����ð� ������ �����
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



// ���༺ ���� ����
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
// ���༺�� ȭ���� ����� �ٽ� ����
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
// �Ѿ˰� � �浹 ���� (5�� ������ � ������� ������ ����)
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
                        }
                    }
                }
            }
        }
    }
}


// �Ѿ˰� �ܰ��� �浹 ���� (10�� ������ �ܰ��� ������� ������ ����)
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
                    }
                }
            }
        }
    }
}



// ���ϰ� ���༺ ���� 
void check_die() {
    float dx, dy;
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            dx = rocket.x - asteroids[i].x;
            dy = rocket.y - asteroids[i].y;
            if (sqrt(dx * dx + dy * dy) < 30 && rocket.active) {
                rocket.active = false;
                rocket.invisible_timer = ROCKET_INVISIBLE_TIME;
                screen_shake_timer = 45;

                life--;
            }
        }
        if (rocket.invisible_timer > 0) {
            rocket.invisible_timer--;
            if (rocket.invisible_timer == 0) rocket.active = true;
        }
    }
}


// ���ϰ� �ܰ��� �Ѿ� ����
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

                life--;
            }
        }
        if (rocket.invisible_timer > 0) {
            rocket.invisible_timer--;
            if (rocket.invisible_timer == 0) rocket.active = true;
        }
    }
}

// ���ϰ� �ܰ��� �浹 ����
void check_die_because_alien() {
    float dx, dy;
    if (alien1.active) {
        dx = rocket.x - alien1.x;
        dy = rocket.y - alien1.y;
        if (sqrt(dx * dx + dy * dy) < 29 && rocket.active) {
            rocket.active = false;
            rocket.invisible_timer = ROCKET_INVISIBLE_TIME;
            screen_shake_timer = 45;

            life--;
        }
    }
    if (rocket.invisible_timer > 0) {
        rocket.invisible_timer--;
        if (rocket.invisible_timer == 0) rocket.active = true;
    }
}

// ���ϰ� �ܰ���2 �浹 ����
void check_die_because_alien2() {
    float dx, dy;
    if (alien2.active) {
        dx = rocket.x - alien2.x;
        dy = rocket.y - alien2.y;
        if (sqrt(dx * dx + dy * dy) < 34 && rocket.active) {
            rocket.active = false;
            rocket.invisible_timer = ROCKET_INVISIBLE_TIME;
            screen_shake_timer = 45;

            life--;
        }
    }
    if (rocket.invisible_timer > 0) {
        rocket.invisible_timer--;
        if (rocket.invisible_timer == 0) rocket.active = true;
    }
}



// display
void draw_scene() {
    // �浹 �� ȭ�� ��鸲 ȿ��
    if (screen_shake_timer > 0) {
        // -10 ~ 10 ������ ������
        shake_offset_x = (rand() % 21) - 10;
        shake_offset_y = (rand() % 21) - 10;
        screen_shake_timer--;
    }
    else {
        shake_offset_x = 0;
        shake_offset_y = 0;
    }
    al_draw_bitmap(background, shake_offset_x, shake_offset_y, 0);

    // STAGE 2 - �ܰ��� ��� �� STAGE2 ���� ȿ��
    if (alien1.active || alien2.active) {
        if (alien1.x < SCREEN_WIDTH / 4 || alien2.y < SCREEN_HEIGHT / 2) {
            al_draw_bitmap(warning, shake_offset_x, shake_offset_y + 40, 0);
        }

    }
    // STAGE 3
    //if (alien2.counts >= 1) {
    //    //if(alien2.counts == 1) 
    //    //al_draw_bitmap(alien2IMG, shake_offset_x + 230, shake_offset_y - 10, 0);
    //    //al_draw_bitmap(alien2IMG, 230, SCREEN_HEIGHT - 900, 0);
    //}

    // �ΰ� display
    if (init_logo_timer > 0) {
        al_draw_bitmap(logo, SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 80, 0);
        init_logo_timer--;
    }
    else al_draw_bitmap(logo, shake_offset_x, shake_offset_y, 0);

    // ���� �Ҳ� - ������ ���� �� display
    if (trail_flag) {
        al_draw_rotated_bitmap(trail, ROCKET_SIZE + 33, ROCKET_SIZE - 25, rocket.x + cos(rocket.angle) * 10, rocket.y + sin(rocket.angle) * 10, rocket.angle - getRadian(65), 0);
        al_draw_rotated_bitmap(trail, ROCKET_SIZE + 43, ROCKET_SIZE - 15, rocket.x + cos(rocket.angle) * 10, rocket.y + sin(rocket.angle) * 10, rocket.angle - getRadian(115), 0);
    }

    // ���� display
    if (rocket.invisible_timer > 0) {
        al_draw_rotated_bitmap(invisible_ship, 10, 10, rocket.x, rocket.y, rocket.angle, 0);
    }
    else al_draw_rotated_bitmap(ship, 10, 10, rocket.x, rocket.y, rocket.angle, 0);

    // �Ѿ�, ���༺ display
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            al_draw_rotated_bitmap(bulletIMG, 10, 10, bullets[i].x + 0.1, bullets[i].y + 0.1, bullets[i].angle, 0);
        }
    }
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            if (!i % 3) al_draw_bitmap(asteroidIMG_large, asteroids[i].x, asteroids[i].y, 0);
            else al_draw_bitmap(asteroidIMG_small, asteroids[i].x, asteroids[i].y, 0);

        }
        else if (asteroids[i].hits >= ASTEROID_HITS) {
            if (!i % 2) al_draw_bitmap(explosion_large, asteroids[i].x - 20, asteroids[i].y - 20, 0);
            else al_draw_bitmap(explosion_small, asteroids[i].x - 20, asteroids[i].y - 20, 0);
        }
    }

    // alien, alien bullets display
    if (alien1.active) {
        al_draw_bitmap(alien1_withUFO, alien1.x, alien1.y, 0);
        for (int i = 0; i < MAX_BULLETS_ALEIN; i++) {
            if (alien1.bullets[i].active) al_draw_bitmap(alien_bullet, alien1.bullets[i].x + 0.1, alien1.bullets[i].y + 0.1, 0);

        }
    } // alien ������ �� display
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
    if (alien2.active) {
        al_draw_bitmap(alien2_withoutUF0, alien2.x, alien2.y, 0);
    }

}