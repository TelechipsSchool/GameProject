#include "game.h"

int get_radius(int type) {
    switch (type) {
    case 1: return 15; break;
    case 2: return 25; break;
    case 3: return 35; break;
    case 4: return 50; break;
    case 5: return 70; break;
    }
}

void merge_planets(Planet* a, Planet* b) {
    a->type += 1;
    a->radius = get_radius(a->type);
    a->velocity = Vector2_Scale(Vector2_Add(a->velocity, b->velocity), 0.5f);
    b->isFlying = false;
    b->type = 0;
    b->velocity = (Vector2){ 0,0 };
}

void start_game() {
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);//0.00025초마다 이벤트 ->매초 4000번. 
    //새 타이머를 가리키는 포인터 반환. or NULL반환
    ALLEGRO_EVENT_QUEUE* game_event_queue = al_create_event_queue();//선입선출
    al_register_event_source(game_event_queue, al_get_mouse_event_source());
    al_register_event_source(game_event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    // 행성 이미지
    ALLEGRO_BITMAP* planet_img1 = load_bitmap_resized("images/planet_1.png", 30, 30);
    ALLEGRO_BITMAP* planet_img2 = load_bitmap_resized("images/planet_2.png", 50, 50);
    ALLEGRO_BITMAP* planet_img3 = load_bitmap_resized("images/planet_3.png", 70, 70);
    ALLEGRO_BITMAP* planet_img4 = load_bitmap_resized("images/planet_4.png", 100, 100);
    ALLEGRO_BITMAP* planet_img5 = load_bitmap_resized("images/planet_5.png", 140, 140);

    // 발사 지점
    Vector2 shootStart = { init_x, init_y };

    // 행성 무작위 생성. 행성 배열 초기화. 
    Planet planet[MAX_PLANET];
    for (int i = 0; i < MAX_PLANET; ++i) {
        planet[i].position = shootStart;
        planet[i].velocity.x = 0;
        planet[i].velocity.y = 0;
        planet[i].isFlying = 0;
        planet[i].type = rand() % 5 + 1;;
        planet[i].radius = get_radius(planet[i].type);
    }
    // 행성 갯수
    int planet_num = 0;//첫번째 행성 0부터시작.

    // 중력, 시작점, 센터 생성
    Vector2 gravityCenter = { center_x, center_y };
    ALLEGRO_BITMAP* center = load_bitmap_resized("images/center.png", 30, 30);
    ALLEGRO_BITMAP* startpoint = load_bitmap_resized("images/ShootStartingPoint.png", 150, 150);
    ALLEGRO_BITMAP* gravityfield = load_bitmap_resized("images/GravityField.png", 700, 700);

    // 변수 선언
    Vector2 dragStart, dragEnd;
    float dragThreshold = 30.0f;
    float gravityScale = 1.0f;
    float centerCoefficient = 0.003f;
    float deltaTime = 1.0f / FPS;
    float last_shot_time = 0.0f;

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
        //마우스를 드래그 놓은 위치 선언.
        
        // 마우스 뗏을 때
        else if (isDragging && !isFired) {
            isDragging = false;
            Vector2 dragVec = Vector2_Sub(dragEnd, dragStart);

            // 유도선 길이만큼 행성한테 힘을 가함 (-인 이유는 드래그 반대방향이므로)
            if (Vector2_Length(dragVec) > dragThreshold) {
                Vector2 force = Vector2_Clamp(Vector2_Scale(dragVec, -dragForce), maxForce);
                planet[planet_num].velocity = force;
                planet[planet_num].position = shootStart;
                planet[planet_num].isFlying = true;
            }

            isFired = true;
            last_shot_time = al_get_time();
        }

        // 행성이 날라가는 도중 계산
        for (int i = 0; i <= planet_num; ++i) {
            if (!planet[i].isFlying || planet[i].type == 0) { continue; }
            else{
                // 중앙 원과 충돌 시
                if (collision_check(planet[i].position.x, planet[i].position.y, planet[i].radius, center_x, center_y, 15)) {
                    Vector2 normal = Vector2_Normalize(Vector2_Sub(planet[i].position, gravityCenter));//중심에서 반대 0벡터
                    Vector2 new_vel  = Vector2_Sub(planet[i].velocity,
                        Vector2_Scale(Vector2_Project(planet[i].velocity, normal), (1 + RESTITUTION)));
                    planet[i].velocity = new_vel;//탄성 추가
                }
                // 행성끼리 충돌 시
                for (int j = 0; j <= planet_num; ++j) {
                    if (i==j || !planet[j].isFlying) continue; // 자기 자신과의 충돌 무시+비활성화행성인지체크

                
                    // 다른 행성과의 충돌
                    if (collision_check(planet[i].position.x, planet[i].position.y, planet[i].radius,
                        planet[j].position.x, planet[j].position.y, planet[j].radius)) {
                       
                        if ((planet[i].type == planet[j].type)&&planet[j].isFlying) {
                            merge_planets(planet + i, planet + j);
                            continue;
                        }

                        // 충돌 방향 벡터 (i에서 j로 향하는 방향)
                        Vector2 normal = Vector2_Normalize(Vector2_Sub(planet[i].position, planet[j].position));

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

        // 발사하고 5초 뒤에 생성
        if (al_get_time() - last_shot_time > 5.0 && isFired) {
            ++planet_num;
            isFired = false;
        }

        // 화면 업데이트 redraw
        if (redraw) {
            al_clear_to_color(al_map_rgb(20, 20, 20));

            // 행성 그리기
            for (int i = 0; i <= planet_num; ++i) {
                ALLEGRO_BITMAP* planet_img = NULL;
                switch (planet[i].type) {
                case 1: planet_img = planet_img1; break;
                case 2: planet_img = planet_img2; break;
                case 3: planet_img = planet_img3; break;
                case 4: planet_img = planet_img4; break;
                case 5: planet_img = planet_img5; break;
               // default: planet_img = planet_img1; break;
                }

                if (planet_img == NULL) continue;
                // position에서 빼기하는 이유는, 중심 좌표에서 시작하면 그림이 밀림
                al_draw_bitmap(planet_img, planet[i].position.x - planet[i].radius, 
                    planet[i].position.y - planet[i].radius, 0);
            }

            // 중심, 스타트, 중력필드 그리기
            al_draw_bitmap(center, center_x - 15, center_y - 15, 0);
            al_draw_bitmap(gravityfield, center_x - 350, center_y - 350, 0);
            al_draw_bitmap(startpoint, init_x - 75, init_y - 75, 0);

            // 드래그 하는 동안, 유도선 그리기
            if (isDragging) {
                Vector2 forceVec = Vector2_Clamp(Vector2_Scale(Vector2_Sub(dragEnd, dragStart), -line_length), maxForce);
                Vector2 endPoint = Vector2_Add(shootStart, forceVec);
                al_draw_line(shootStart.x, shootStart.y, endPoint.x, endPoint.y, al_map_rgb(255, 255, 0), 2);
            }

            // 화면 갱신
            al_flip_display();
        }
    }

    // 해제
    al_destroy_bitmap(planet_img1);
    al_destroy_bitmap(center);
    al_destroy_bitmap(gravityfield);
    al_destroy_bitmap(startpoint);
    al_destroy_event_queue(game_event_queue);
}

// 충돌 체크 함수
bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < size1 + size2;
}

// 핵심 중력 + 회전 보정 함수
void CalcGravity(Planet* rb, Vector2 center, float centerCoefficient, float deltaTime) {
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