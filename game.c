#include "game.h"

void start_game() {
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* game_event_queue = al_create_event_queue();
    al_register_event_source(game_event_queue, al_get_mouse_event_source());
    al_register_event_source(game_event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    ALLEGRO_BITMAP* planet_img = load_bitmap_resized("images/planet_1.png", 30, 30);
    Vector2 shootStart = { init_x, init_y };
    //Rigidbody2D planet = { shootStart, {0, 0}, 0 };

    Rigidbody2D planet[MAX_PLANET];
    for (int i = 0; i < MAX_PLANET; ++i) {
        planet[i].position = shootStart;
        planet[i].velocity.x = 0;
        planet[i].velocity.y = 0;
        planet[i].isFlying = 0;
    }
    int planet_num = 0;

    Vector2 gravityCenter = { center_x, center_y };
    ALLEGRO_BITMAP* center = load_bitmap_resized("images/center.png", 30, 30);
    ALLEGRO_BITMAP* startpoint = load_bitmap_resized("images/ShootStartingPoint.png", 150, 150);
    ALLEGRO_BITMAP* gravityfield = load_bitmap_resized("images/GravityField.png", 500, 500);

    Vector2 dragStart, dragEnd;
    float maxForce = 70.0f;
    float dragThreshold = 30.0f;
    float gravityScale = 1.0f;
    float centerCoefficient = 0.03f;
    float deltaTime = 1.0f / FPS;
    float last_shot_time = 0.0f;

    float angle = 0.0;
    bool isFired = false, redraw = true, playing = true, isDragging = false;

    while (playing) {
        ALLEGRO_EVENT event;
        al_wait_for_event(game_event_queue, &event);

        // 화면 닫을 때
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            playing = 0;
        // 화면 새로고침 주기
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = 1;
        }
        
        ALLEGRO_MOUSE_STATE mouse;
        al_get_mouse_state(&mouse);

        // 마우스 누를 때
        if (al_mouse_button_down(&mouse, 1) && !isFired) {
            if (!isDragging) {
                isDragging = true;
                dragStart.x = mouse.x;
                dragStart.y = mouse.y;
            }
            dragEnd.x = mouse.x;
            dragEnd.y = mouse.y;
        }

        // 마우스 뗏을 때
        else if (isDragging && !isFired) {
            isDragging = false;
            Vector2 dragVec = Vector2_Sub(dragEnd, dragStart);

            // 유도선 길이만큼 행성한테 힘을 가함
            if (Vector2_Length(dragVec) > dragThreshold) {
                Vector2 force = Vector2_Clamp(Vector2_Scale(dragVec, -1), maxForce);
                planet[planet_num].velocity = force;
                planet[planet_num].position = shootStart;
                planet[planet_num].isFlying = true;
            }

            isFired = true;
            last_shot_time = al_get_time();
        }

        // 행성이 날라가는 도중 계산
        for (int i = 0; i <= planet_num; ++i) {
            if (planet[i].isFlying) {
                // 중앙 원과 충돌 시 반발
                if (collision_check(planet[i].position.x, planet[i].position.y, 30, center_x, center_y, 30)) {
                    Vector2 normal = Vector2_Normalize(Vector2_Sub(planet[i].position, gravityCenter));
                    planet[i].velocity = Vector2_Sub(planet[i].velocity, Vector2_Scale(Vector2_Project(planet[i].velocity, normal), (1 + RESTITUTION)));
                }
                // 행성끼리 충돌
                for (int j = 0; j <= planet_num; ++j) {
                    if (i == j) continue; // 자기 자신과의 충돌 무시

                    // 다른 행성과의 충돌
                    if (collision_check(planet[i].position.x, planet[i].position.y, 30,
                        planet[j].position.x, planet[j].position.y, 30)) {

                        // 충돌 방향 벡터 (i에서 j로 향하는 방향)
                        Vector2 normal = Vector2_Normalize(Vector2_Sub(planet[j].position, planet[i].position));

                        // i와 j의 속도 투영값 계산
                        Vector2 vel_i_proj = Vector2_Project(planet[i].velocity, normal);
                        Vector2 vel_j_proj = Vector2_Project(planet[j].velocity, normal);

                        // 탄성 충돌 공식을 사용하여 새로운 속도 계산
                        Vector2 new_vel_i = Vector2_Sub(planet[i].velocity,
                            Vector2_Scale(vel_i_proj, (1 + RESTITUTION)));
                        Vector2 new_vel_j = Vector2_Sub(planet[j].velocity,
                            Vector2_Scale(vel_j_proj, (1 + RESTITUTION)));

                        // 속도 반영
                        planet[i].velocity = new_vel_i;
                        planet[j].velocity = new_vel_j;
                    }
                }

                // 중력 계산
                CalcGravity(&planet[i], gravityCenter, centerCoefficient, deltaTime);
                planet[i].position = Vector2_Add(planet[i].position, Vector2_Scale(planet[i].velocity, deltaTime));
            }
        }

        if (al_get_time() - last_shot_time > 5.0 && isFired) {
            ++planet_num;
            isFired = false;
        }

        // 화면 업데이트 redraw
        if (redraw) {
            al_clear_to_color(al_map_rgb(20, 20, 20));

            for (int i = 0; i <= planet_num; ++i) {
                al_draw_bitmap(planet_img, planet[i].position.x - 15, planet[i].position.y - 15, 0);
            }

            al_draw_bitmap(center, center_x - 15, center_y - 15, 0);
            al_draw_bitmap(gravityfield, center_x - 250, center_y - 250, 0);
            al_draw_bitmap(startpoint, init_x - 75, init_y - 75, 0);

            if (isDragging) {
                Vector2 forceVec = Vector2_Clamp(Vector2_Scale(Vector2_Sub(dragEnd, dragStart), -line_length), maxForce);
                Vector2 endPoint = Vector2_Add(shootStart, forceVec);
                al_draw_line(shootStart.x, shootStart.y, endPoint.x, endPoint.y, al_map_rgb(255, 255, 0), 2);
            }

            al_flip_display();
        }
    }

    // 해제
    al_destroy_bitmap(planet_img);
    al_destroy_bitmap(center);
    al_destroy_bitmap(gravityfield);
    al_destroy_bitmap(startpoint);
    al_destroy_event_queue(game_event_queue);
}

bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (size1 + size2) / 2;
}

// 핵심 중력 + 회전 보정 함수
void CalcGravity(Rigidbody2D* rb, Vector2 center, float centerCoefficient, float deltaTime) {
    Vector2 gravityDir = Vector2_Normalize(Vector2_Sub(center, rb->position));
    Vector2 gravity = Vector2_Scale(gravityDir, GRAVITY);

    Vector2 vel = rb->velocity;
    Vector2 nextPos = Vector2_Add(rb->position, vel);

    Vector2 parallelVel = Vector2_Project(vel, gravityDir);
    Vector2 perpendicularVel = Vector2_Sub(vel, parallelVel);

    Vector2 correctionDir = Vector2_Project((Vector2) { -perpendicularVel.x, -perpendicularVel.y }, Vector2_Sub(nextPos, center));
    Vector2 newPerpendicularVel = Vector2_Add(perpendicularVel, correctionDir);

    perpendicularVel.x = (1 - centerCoefficient) * perpendicularVel.x + centerCoefficient * newPerpendicularVel.x;
    perpendicularVel.y = (1 - centerCoefficient) * perpendicularVel.y + centerCoefficient * newPerpendicularVel.y;

    rb->velocity = Vector2_Add(Vector2_Add(parallelVel, perpendicularVel), Vector2_Scale(gravity, deltaTime));
}