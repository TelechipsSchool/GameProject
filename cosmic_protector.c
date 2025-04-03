#include "cosmic_protector.h"

Rocket rocket = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0 };
Bullet bullets[MAX_BULLETS] = { 0 };
Asteroid asteroids[MAX_ASTEROIDS] = { 0 };  // ť�� �ٲ㼭 �޸� ���� ���� ���ֱ�
Alien alien1 = { 0 };


bool key_state[ALLEGRO_KEY_MAX] = { false };
bool trail_flag = false; 

ALLEGRO_TIMER* timer = 0;


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
ALLEGRO_BITMAP* alien1IMG = NULL;
ALLEGRO_BITMAP* alien_bullet = NULL;


// ȭ�� ��鸲 ȿ��
int screen_shake_timer = 0;
float shake_offset_x = 0;
float shake_offset_y = 0;

float init_logo_timer = 600;

double game_start_time;
double alien_start_time;
double tt;


int main() {
    // ���߿� ����ȭ�� ���� �� �κ� init.c�� ��ġ��
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_image_addon();
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
            alien_update_bullets();
            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_scene();
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
    alien1IMG = load_bitmap_resized("gfx/alien_effect.jpg", 225, 225);
    alien_bullet = al_load_bitmap("gfx/small_bullet.png");

    ALLEGRO_BITMAP* arr[] = { background, ship, explosion_large, explosion_small, bulletIMG, 
        asteroidIMG_large, asteroidIMG_small, invisible_ship, trail, logo, alien1_withUFO, alien1IMG, alien_bullet };
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
        if(arr[i] == NULL) DEBUG_MSG("%s bitmap is NULL\n", arr[i]);
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
    al_destroy_bitmap(alien1IMG);
    al_destroy_bitmap(alien_bullet);
}

double getRadian(int num) {
    return num * (PI / 180);
}

// ���� ������ �̵�
void update_rocket() {
    trail_flag = true;
    if (rocket.x < 0 || rocket.x > SCREEN_WIDTH || rocket.y < 0 || rocket.y > SCREEN_HEIGHT) {}
    else {
        rocket.x -= sin(-rocket.angle) * 1.5;
        rocket.y -= cos(-rocket.angle) * 1.5;
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

// �ܰ��� �Ѿ� ����
void alien_bullets() {
    for (int i = 0; i < MAX_BULLETS_ALLEIN ; i++) {
        if (alien1.active) {
            alien1.bullets[i].x = alien1.x + cos(alien1.angle) * ROCKET_SIZE;
            alien1.bullets[i].y = alien1.y + sin(alien1.angle) * ROCKET_SIZE;
            alien1.bullets[i].dx = cos(-alien1.angle) * BULLET_SPEED_ALLEIN;   // cos(-alien1.angle) * BULLET_SPEED;
            alien1.bullets[i].dy = sin(-alien1.angle) * BULLET_SPEED_ALLEIN;
            alien1.bullets[i].active = true;
            break;
        }
    }
}

// �ܰ��� �Ѿ� ����
void alien_update_bullets() {
    for (int i = 0; i < MAX_BULLETS_ALLEIN ; i++) {
        if (alien1.bullets[i].active && i%2) {
            alien1.bullets[i].x -= alien1.bullets[i].dy;
            alien1.bullets[i].y -= alien1.bullets[i].dx;
            if (!alien1.active) {
                alien1.bullets[i].active = false;
            }
        }
    }
}


// ���༺ ���� ����
void spawn_asteroids() {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active && (rand() % 100 >= 20 )) {
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
                    if (sqrt(dx * dx + dy * dy) < 30) {
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
// ���ϰ� ���༺ ���� 
void check_die() {
    float dx, dy;
	for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            dx = rocket.x - asteroids[i].x;
            dy = rocket.y - asteroids[i].y;
            if (sqrt(dx * dx + dy * dy) < 26 && rocket.active) {
                rocket.active = false;
                rocket.invisible_timer = ROCKET_INVISIBLE_TIME;
                screen_shake_timer = 45;
            }
        }
        if (rocket.invisible_timer > 0) {
            rocket.invisible_timer--;
            if (rocket.invisible_timer == 0) rocket.active = true;
        }
	}
}

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
        screen_shake_timer += 55;
        alien_bullets(); // ���⼭ �Ѿ� ���� ȣ��
    }
}

void alien_update() {
    if (alien1.active) {
        alien1.x += alien1.dx;
        alien1.angle -= getRadian(0.7); // �̰͵� ȭ�� �߽����� ����� ���� �޾Ƽ� ���� ��������� �ҵ�.
        if (alien1.x >= SCREEN_WIDTH) {
            alien1.active = false;
            screen_shake_timer = 0;
        }
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
    if (alien1.active) {
        al_draw_bitmap(alien1IMG, shake_offset_x, shake_offset_y, 0);
    }

    // �ΰ� display
    if (init_logo_timer > 0) {
        al_draw_bitmap(logo, SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 80, 0);
        init_logo_timer--;
    }
    else al_draw_bitmap(logo, 0, 0, 0);

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
            al_draw_rotated_bitmap(bulletIMG, 10, 10, bullets[i].x + 0.1, bullets[i].y+0.1, rocket.angle, 0);
        }
    }
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            if (!i % 3) al_draw_bitmap(asteroidIMG_large, asteroids[i].x, asteroids[i].y, 0);
            else al_draw_bitmap(asteroidIMG_small, asteroids[i].x, asteroids[i].y, 0);

        }
        else if (asteroids[i].hits >= ASTEROID_HITS) {
            if(!i % 2) al_draw_bitmap(explosion_large, asteroids[i].x - 20, asteroids[i].y - 20, 0);
            else al_draw_bitmap(explosion_small, asteroids[i].x - 20, asteroids[i].y - 20, 0);
        }
    }

    // alien display
    if (alien1.active) {
        al_draw_bitmap(alien1_withUFO, alien1.x, alien1.y, 0);
    }

    // alien �Ѿ� display
    for (int i = 0; i < MAX_BULLETS_ALLEIN; i++) {
        if (alien1.active) {
            //al_draw_bitmap(alien_bullet, alien1.bullets[i].x + 0.1, alien1.bullets[i].y + 0.1, 0);
            //al_draw_rotated_bitmap(alien_bullet, 10, 10, alien1.bullets[i].x, alien1.bullets[i].y, alien1.angle, 0);
            al_draw_bitmap(alien_bullet, alien1.bullets[i].x + 0.1, alien1.bullets[i].y + 0.1, 0);

        }
    }

}