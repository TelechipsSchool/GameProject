#include "cosmic_protector.h"

Rocket rocket = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0 };
Bullet bullets[MAX_BULLETS] = { 0 };
Asteroid asteroids[MAX_ASTEROIDS] = { 0 };  // ť�� �ٲ㼭 �޸� ���� ���� ���ֱ�

bool key_state[ALLEGRO_KEY_MAX] = { false };

ALLEGRO_BITMAP* background = NULL;
ALLEGRO_BITMAP* ship = NULL;
ALLEGRO_BITMAP* explosion_large = NULL;
ALLEGRO_BITMAP* explosion_small = NULL;
ALLEGRO_BITMAP* bulletIMG = NULL;
ALLEGRO_BITMAP* asteroidIMG_large = NULL;
ALLEGRO_BITMAP* asteroidIMG_small = NULL;


int main() {
    // ���߿� ����ȭ�� ���� �� �κ� init.c�� ��ġ��
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_image_addon();

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    ALLEGRO_EVENT event;

    loadBitmap();

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    bool running = true;
    bool redraw = true;
    al_start_timer(timer);

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
                rocket.angle += getRadian(5.7);   
            }// ���� 10% �̵�. y�� ���� 0.1
            if (key_state[ALLEGRO_KEY_RIGHT]) {
                rocket.angle -= getRadian(5.7);   
            }
            update_bullets();
            update_asteroids();
            check_collisions();
            spawn_asteroids();
            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_scene();
            al_flip_display();
        }
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

    ALLEGRO_BITMAP* arr[] = { background, ship, explosion_large, explosion_small, bulletIMG, asteroidIMG_large, asteroidIMG_small };
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
}

double getRadian(int num) {
    return num * (PI / 180);
}

// ���� ������ �̵�
void update_rocket() {
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
// ���༺ ���� ����
void spawn_asteroids() {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active && (rand() % 50 == 0)) {
            asteroids[i].x = rand() % SCREEN_WIDTH;
            asteroids[i].y = rand() % SCREEN_HEIGHT;
            asteroids[i].dx = (rand() % 5 - 2) * 2;
            asteroids[i].dy = (rand() % 5 - 2) * 2;
            asteroids[i].hits = 0;
            asteroids[i].active = true;
        }
    }
}
// ���༺�� ȭ���� ����� �ٽ� ����
void update_asteroids() {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            asteroids[i].x += asteroids[i].dx;
            asteroids[i].y += asteroids[i].dy;
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
                    if (sqrt(dx * dx + dy * dy) < 20) {
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

// display
void draw_scene() {
    al_draw_bitmap(background, 0, 0, 0);
    al_draw_rotated_bitmap(ship, 10, 10, rocket.x, rocket.y, rocket.angle, 0);

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
            if(i % 2) al_draw_bitmap(explosion_large, asteroids[i].x - 20, asteroids[i].y - 20, 0);
            else al_draw_bitmap(explosion_small, asteroids[i].x - 20, asteroids[i].y - 20, 0);
        }
    }
}