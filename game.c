#include "game.h"
#include <math.h>

void start_game() {
    bool playing = true;

    // 이벤트 큐 생성
    ALLEGRO_EVENT_QUEUE* game_event_queue = al_create_event_queue();
    al_register_event_source(game_event_queue, al_get_keyboard_event_source());

    // 행성과 목표 생성
    Planet planet = create_planet(init_x, init_y, 0, 0, 30, 30, "images/planet_1.png");
    ALLEGRO_BITMAP* goal = load_bitmap_resized("images/center.png", 30, 30);
    ALLEGRO_BITMAP* startpoint = load_bitmap_resized("images/ShootStartingPoint.png", 150, 150);
    ALLEGRO_BITMAP* gravityfield = load_bitmap_resized("images/GravityField.png", 300, 300);

    float angle = 0.;

    while (playing) {
        ALLEGRO_EVENT event;
        while (al_get_next_event(game_event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    playing = false; // ESC 키로 게임 종료
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                    angle -= 5.0f; // 위 방향으로 각도 증가
                    if (angle < 0) angle += 360.0f;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                    angle += 5.0f; // 아래 방향으로 각도 감소
                    if (angle >= 360) angle -= 360.0f;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    planet.angle = angle; // 발사 각도 설정
                    planet.speed = 0.1;
                }
            }
        }       

        // 가운데 점과 충돌했을 때
        if (collision_check(planet.x, planet.y, 30, goal_x, goal_y, 30)) {
            // 구현못함..
        }

        // 중력 필드에 들어왔을 때
        if (collision_check(planet.x, planet.y, 30, goal_x, goal_y, 300)) {
            // 구현못함..
        }

        // 발사된 행성 위치 업데이트 (각도에 따라)
        planet.x += planet.speed * cos(planet.angle * ALLEGRO_PI / 180.0f);
        planet.y -= planet.speed * sin(planet.angle * ALLEGRO_PI / 180.0f);

        // 화면 초기화 (회색)
        al_clear_to_color(al_map_rgb(20, 20, 20));

        // 그리기
        al_draw_bitmap(planet.image, planet.x - planet.width / 2, planet.y - planet.height / 2, 0);
        al_draw_bitmap(goal, goal_x - 15, goal_y - 15, 0);
        al_draw_bitmap(gravityfield, goal_x - 150, goal_y - 150, 0);
        al_draw_bitmap(startpoint, init_x - 75, init_y - 75, 0);

        // 유도선 그리기: 발사 각도 방향으로 선 그리기
        int x1 = init_x;
        int y1 = init_y;
        int x2 = init_x + line_length * cos(angle * ALLEGRO_PI / 180.0f);
        int y2 = init_y - line_length * sin(angle * ALLEGRO_PI / 180.0f);
        al_draw_line(x1, y1, x2, y2, al_map_rgb(255, 255, 0), 1);  // 유도선 색상: 노란색

        // display 업데이트
        al_flip_display();
    }

    al_destroy_bitmap(planet.image);
    al_destroy_bitmap(goal);
    al_destroy_event_queue(game_event_queue);
}

Planet create_planet(float x, float y, float speed, float angle, int width, int height, const char* image_path) {
    Planet planet;
    planet.x = x;
    planet.y = y;
    planet.speed = speed;
    planet.angle = angle;
    planet.width = width;
    planet.height = height;
    planet.image = load_bitmap_resized(image_path, width, height);

    return planet;
}

bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2) {
    // 두 원의 중심 간 거리 계산
    int dx = x2 - x1;
    int dy = y2 - y1;
    float distance = sqrt(dx * dx + dy * dy);

    // 두 원의 반지름 합이 두 원의 중심 간 거리보다 크거나 같으면 충돌
    if (distance < (size1 + size2) / 2) {
        return true; // 충돌
    }
    else {
        return false; // 충돌하지 않음
    }
}