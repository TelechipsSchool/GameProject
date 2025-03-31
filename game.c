#include "game.h"

// 행성 배열
Planet* planet_list[MAX_PLANET] = { 0 };
int planet_num = 0;         // 행성 갯수, 첫번째 행성 0부터시작.

void start_game() {
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);//0.00025초마다 이벤트 ->매초 4000번. 
    //새 타이머를 가리키는 포인터 반환. or NULL반환
    ALLEGRO_EVENT_QUEUE* game_event_queue = al_create_event_queue();//선입선출
    al_register_event_source(game_event_queue, al_get_mouse_event_source());
    al_register_event_source(game_event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    // 행성 이미지
    ALLEGRO_BITMAP* planet_img1 = load_bitmap_resized("images/planet_1.png", 30, 30);
    ALLEGRO_BITMAP* planet_img2 = load_bitmap_resized("images/planet_2.png", 40, 40);
    ALLEGRO_BITMAP* planet_img3 = load_bitmap_resized("images/planet_3.png", 60, 60);
    ALLEGRO_BITMAP* planet_img4 = load_bitmap_resized("images/planet_4.png", 90, 90);
    ALLEGRO_BITMAP* planet_img5 = load_bitmap_resized("images/planet_5.png", 120, 120);
    ALLEGRO_BITMAP* planet_img6 = load_bitmap_resized("images/planet_6.png", 160, 160);
    ALLEGRO_BITMAP* planet_img7 = load_bitmap_resized("images/planet_7.png", 300, 300);
    ALLEGRO_BITMAP* home_icon = load_bitmap_resized("images/home.png", 50, 50);

    // 발사 지점
    Vector2 shootStart = { init_x, init_y };
    
    planet_list[planet_num++] = Create_Planet(shootStart, (Vector2) { 0, 0 }, rand() % 3 + 1);        // 발사 행성은 type 3 까지만 나옴

    // 중력, 시작점, 센터 생성
    Vector2 gravityCenter = { center_x, center_y };
    ALLEGRO_BITMAP* center = load_bitmap_resized("images/center.png", 30, 30);
    ALLEGRO_BITMAP* startpoint = load_bitmap_resized("images/ShootStartingPoint.png", 150, 150);
    ALLEGRO_BITMAP* gravityfield = load_bitmap_resized("images/GravityField.png", 700, 700);

    if (!startpoint) printf("출발지점 이미지 로딩 실패\n");


    // 변수 선언
    Vector2 dragStart, dragEnd;
    float dragThreshold = 30.0f;
    float gravityScale = 1.0f;
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
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_R) {
                playing = false;
                start_game(); // 재시작
                return;
            }
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            int mx = event.mouse.x;
            int my = event.mouse.y;

            // home 버튼 위치 클릭됐는지 확인
            if (mx >= SCREEN_W - 60 && mx <= SCREEN_W - 10 &&
                my >= 10 && my <= 60) {

                playing = false;
                // 리소스 정리 후 메뉴로 복귀
                for (int i = 0; i < planet_num; ++i) {
                    if (planet_list[i]) free(planet_list[i]);
                    
                }

                al_destroy_bitmap(planet_img1);
                al_destroy_bitmap(planet_img2);
                al_destroy_bitmap(planet_img3);
                al_destroy_bitmap(planet_img4);
                al_destroy_bitmap(planet_img5);
                al_destroy_bitmap(planet_img6);
                al_destroy_bitmap(planet_img7);
                al_destroy_bitmap(center);
                al_destroy_bitmap(gravityfield);
                al_destroy_bitmap(startpoint);
                al_destroy_bitmap(home_icon);
                al_destroy_event_queue(game_event_queue);
                al_destroy_timer(timer);

                return; // start_game() 종료 → main 메뉴 화면으로 복귀
            }
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
            if (Vector2_Length(dragVec) > dragThreshold && planet_num <MAX_PLANET) {
                Vector2 force = Vector2_Clamp(Vector2_Scale(dragVec, -dragForce), maxForce);
               
                Planet* p = planet_list[planet_num - 1];
                p->velocity = force;
                p->isFlying = true;
               }

            isFired = true;
            last_shot_time = al_get_time();
        }

        // 행성이 날라가는 도중 계산
        for (int i = 0; i < planet_num;) {
            Planet* p = planet_list[i];
            if (p == NULL) { ++i; continue; }

            //화면 밖으로 나간 행성 제거
            if (p->position.x<-100 || p->position.x>SCREEN_W + 100 || p->position.y< -100 || p->position.y>SCREEN_H + 100) {
                Destroy_Planet(planet_list, &planet_num, i);
                continue;
            }

            if (!p->isFlying || p->type == 0) { ++i; continue; }


            // 중앙 원과 충돌 시
            if (collision_check(p->position.x, p->position.y, p->radius, center_x, center_y, 15)) {
                Vector2 normal = Vector2_Normalize(Vector2_Sub(p->position, gravityCenter));//중심에서 반대 0벡터
                // 1. 위치 보정 (겹침 해소)
                float distance = Vector2_Distance(p->position, (Vector2) { center_x, center_y });
                float penetrationDepth = (p->radius + 15) - distance; // 겹친 거리 계산

                if (penetrationDepth > 0) { // 이미 겹쳐 있는 경우 밀쳐내기
                    Vector2 correction = Vector2_Scale(normal, penetrationDepth);
                    p->position = Vector2_Add(p->position, correction);
                }

                p->velocity = Vector2_Sub(p->velocity,
                    Vector2_Scale(Vector2_Project(p->velocity, normal), (1 + RESTITUTION * 0.5)));      // 탄성 계수 줄임.
            }

            for (int j = 0; j < planet_num; ++j) {
                Planet* q = planet_list[j];
                if (q == NULL || q == p || !q->isFlying) continue; // 자기 자신과의 충돌 무시+비활성화 행성 체크

                if (collision_check(p->position.x, p->position.y, p->radius,
                    q->position.x, q->position.y, q->radius)) {

                    if (p->type == q->type) {
                        merge_planets(p, q);
                        Destroy_Planet(planet_list, &planet_num, j);
                        if (j < i)i--;//인덱스 조정
                        continue;
                    }

                    // 충돌 방향 벡터 (a에서 q로 향하는 방향)
                    Vector2 normal = Vector2_Normalize(Vector2_Sub(p->position, q->position));

                    // 위치 보정 (겹침 해소)
                    float distance = Vector2_Distance(p->position, q->position);
                    float penetrationDepth = (p->radius + q->radius) - distance; // 겹친 거리 계산

                    if (penetrationDepth > 0) { // 이미 겹쳐 있는 경우 밀쳐내기
                        Vector2 correction = Vector2_Scale(normal, penetrationDepth * 1.05f);
                        q->position = Vector2_Sub(q->position, correction);
                    }

                    // 속도 업데이트 (탄성 충돌 반영)
                    p->velocity = Vector2_Sub(p->velocity, Vector2_Scale(Vector2_Project(p->velocity, normal), (1 + RESTITUTION)));
                    q->velocity = Vector2_Sub(q->velocity, Vector2_Scale(Vector2_Project(q->velocity, normal), (1 + RESTITUTION)));
                }
            }
            if (p->isFlying) {
                // 중력 계산
                CalcGravity(p, gravityCenter, centerCoefficient, deltaTime);
                p->position = Vector2_Add(p->position, Vector2_Scale(p->velocity, deltaTime));
                ++i;
            }
        }

        // 발사하고 5초 뒤에 생성
        if (al_get_time() - last_shot_time > 5.0 && isFired) {
            if (planet_num < MAX_PLANET) {
                planet_list[planet_num++] = Create_Planet(shootStart, (Vector2) { 0, 0 }, rand() % 3 + 1);
            }
            isFired = false;
        }

        // 화면 업데이트 redraw
        if (redraw) {
            al_clear_to_color(al_map_rgb(20, 20, 20));

            //1.중력필드 배경, 2. 출발 지점, 3. 중심 순서로 그림
            al_draw_bitmap(gravityfield, center_x - 350, center_y - 350, 0);
            al_draw_bitmap(startpoint, init_x - 75, init_y - 75, 0);
            al_draw_bitmap(center, center_x - 15, center_y - 15, 0);
            al_draw_bitmap(home_icon, SCREEN_W - 60, 10, 0);

            // 행성들 그리기
            for (int i = 0; i <= planet_num; ++i) {
                Planet* p = planet_list[i];
                if (!p) continue;

                ALLEGRO_BITMAP* planet_img = NULL;
                if (al_get_time() < p->explode_time) {
                    planet_img = load_bitmap_resized("images/explode.png", p->radius * 2, p->radius * 2);;
                }
                else {
                    switch (p->type) {
                    case 1: planet_img = planet_img1; break;
                    case 2: planet_img = planet_img2; break;
                    case 3: planet_img = planet_img3; break;
                    case 4: planet_img = planet_img4; break;
                    case 5: planet_img = planet_img5; break;
                    case 6: planet_img = planet_img6; break;
                    case 7: planet_img = planet_img7; break;
                    }
                }

                if (planet_img == NULL) continue;
                // position에서 빼기하는 이유는, 중심 좌표에서 시작하면 그림이 밀림
                al_draw_bitmap(planet_img, p->position.x - p->radius,
                    p->position.y - p->radius, 0);
            }


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
    for (int i = 0; i < planet_num; ++i) {
        if (planet_list[i]) free(planet_list[i]);//0만 false반환. 0아니면 true
    }

    // 해제
    al_destroy_bitmap(planet_img1);
    al_destroy_bitmap(planet_img2);
    al_destroy_bitmap(planet_img3);
    al_destroy_bitmap(planet_img4);
    al_destroy_bitmap(planet_img5);
    al_destroy_bitmap(planet_img6);
    al_destroy_bitmap(planet_img7);
    al_destroy_bitmap(center);
    al_destroy_bitmap(gravityfield);
    al_destroy_bitmap(startpoint);
    al_destroy_bitmap(home_icon);
    al_destroy_event_queue(game_event_queue);
    al_destroy_timer(timer);
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

Planet* Create_Planet(Vector2 pos, Vector2 vel, int type) {
    Planet* p = (Planet*)calloc(1,sizeof(Planet));
    if (!p) {
        printf("행성 생성 실패 : 메모리 부족\n");
        return 0;
    }
    p->position = pos;
    p->velocity = vel;
    p->type = type;
    p->radius = get_radius(type);
    p->isFlying = false;
    p->explode_time = 0.;
    return p;    
}

void Destroy_Planet(Planet** planet_list, int* count, int index) {
    if (!planet_list[index]) return;//0이면 없는 상태.
    free(planet_list[index]);
    //뒤 행성들을 앞으로 당겨서 빈칸 없애기.
    for (int i = index; i < (*count) - 1; ++i) {
        planet_list[i] = planet_list[i + 1];
    }
    planet_list[*count - 1] =NULL;
    (*count)--;
}

int get_radius(int type) {
    switch (type) {
    case 1: return 15; break;
    case 2: return 20; break;
    case 3: return 30; break;
    case 4: return 45; break;
    case 5: return 60; break;
    case 6: return 80; break;
    case 7: return 150; break;
    }
}

void merge_planets(Planet* a, Planet* b) {
    a->type += 1;
    if (a->type > PLANET_TYPES) {
        printf("win!\n");
    }
    a->radius = get_radius(a->type);
    //a->velocity = Vector2_Scale(Vector2_Add(a->velocity, b->velocity), 0.5f);

    a->explode_time = al_get_time() + 0.5;      // 폭발 유지 2초

    b->isFlying = false;
    b->type = 0;
    b->velocity = (Vector2){ 0,0 };
}