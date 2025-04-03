#include "game.h"


void start_game(char* username) {
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* game_event_queue;
    ALLEGRO_SAMPLE* pull_sound;
    ALLEGRO_SAMPLE* release_sound;
    ALLEGRO_BITMAP* scroll_frames[SCROLL_FRAMES];
    int current_scroll_frame = 0;

    init_game_resources(&timer, &game_event_queue);
    load_game_sounds(&pull_sound, &release_sound);
    load_scroll_background(scroll_frames);

    // 행성 이미지
    load_planet_images();
        
    // 발사 지점
    Vector2 shootStart = { init_x, init_y };
    Vector2 waitPoint = { wait_x, wait_y };
    
    int max_type = get_max_type_by_difficulty(current_difficulty);
    create_initial_planets(shootStart, waitPoint, max_type);

    // 중력, 시작점, 센터 생성
    Vector2 gravityCenter = { center_x, center_y };
    ALLEGRO_BITMAP* center = load_bitmap_resized("images/center.png", 30, 30);
    ALLEGRO_BITMAP* startpoint = load_bitmap_resized("images/ShootStartingPoint.png", 150, 150);
    ALLEGRO_BITMAP* gravityfield = load_bitmap_resized("images/GravityField.png", 700, 700);

    if (!startpoint) DEBUG_MSG(startpoint - 이미지 로드 실패);

    // 변수 선언
    Vector2 dragStart, dragEnd;
    float dragThreshold = 30.0f;
    float gravityScale = 1.0f;
    float deltaTime = 1.0f / FPS;
    float last_shot_time = 0.0f;

    bool isFired = false, redraw = true, playing = true, isDragging = false;
    bool paused = false;

    int high_score = get_high_score();

    ALLEGRO_SAMPLE_INSTANCE* game_bgm = init_bgm("audio/start.ogg");

    while (playing) {
        ALLEGRO_EVENT event;
        al_wait_for_event(game_event_queue, &event);        
        
        // ESC 키를 눌렀을 때 게임 종료
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {               
                //printf("%s %d\n", username, score);       // 디버깅 용

                al_set_sample_instance_playing(game_bgm, false);

                save_score(username, score);
                reset_planets();
                playing = false;
                play_music("audio/cancel.ogg");
                menu();
                return;                
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                play_music("audio/switch.ogg");
                paused = !paused;  // true <-> false 전환
                al_set_sample_instance_playing(game_bgm, !paused);
  
            }
        }

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
                // 마우스 처음 눌렀을 때만 재생
                al_play_sample(pull_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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
               
                Planet* p = planet_list[planet_num - 2];
                p->velocity = force;
                p->isFlying = true;
                isFired = true;
                last_shot_time = al_get_time();
                // 마우스 처음 눌렀을 때만 재생
                al_play_sample(release_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
               }           
        }

        if (!paused) {

            // 행성이 날라가는 도중 계산
            for (int i = 0; i < planet_num;) {
                Planet* p = planet_list[i];
                if (p == NULL) { ++i; continue; }

                //화면 밖으로 나간 행성 제거
                if (p->position.x<-100 || p->position.x>SCREEN_W + 100 || p->position.y< -100 || p->position.y>SCREEN_H + 100) {
                    Destroy_Planet(planet_list, &planet_num, i);
                    // 점수 깎임
                    score -= 100;
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

                float distance = Vector2_Length(Vector2_Sub(p->position, gravityCenter));

                // 먼저 wasInGravityZone 조건: 완전히 안쪽까지 들어갔을 때만 true
                if (distance < 280.0f) {
                    p->wasInGravityZone = true;
                }

                // 그 이후에 나가려는 상태 체크
                if (p->wasInGravityZone && distance >= 300.0f && distance <= 350.0f) {
                    p->isLeavingGravityZone = true;
                }
                else {
                    p->isLeavingGravityZone = false;
                }

                for (int j = 0; j < planet_num; ++j) {
                    Planet* q = planet_list[j];
                    if (q == NULL || q == p || !q->isFlying) continue; // 자기 자신과의 충돌 무시+비활성화 행성 체크

                    if (collision_check(p->position.x, p->position.y, p->radius,
                        q->position.x, q->position.y, q->radius)) {

                        if (p->type == q->type) {
                            score += p->radius + q->radius;
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
                    CalcGravity(p, gravityCenter, deltaTime);
                    p->position = Vector2_Add(p->position, Vector2_Scale(p->velocity, deltaTime));
                    p->rotation += p->angularVelocity * deltaTime;
                    ++i;
                }
            }

            // 발사하고 5초 뒤에 생성
            if (al_get_time() - last_shot_time > 5.0 && isFired) {
                // 발사 생성하기 전에 태양 있는지 확인
                // 승리 조건
                for (int i = 0; i < planet_num; ++i) {
                    Planet* p = planet_list[i];
                    if (p->type >= PLANET_TYPES) {
                        save_score(username, score);

                        // 해제 먼저!
                        for (int i = 0; i < planet_num; ++i) {
                            Destroy_Planet(planet_list, &planet_num, i);
                        }

                        // 안전 초기화
                        for (int i = 0; i < MAX_PLANET; ++i) planet_list[i] = NULL;
                        planet_num = 0;
                        playing = false;
                        score = 0;
                        story2();
                    }
                }

                if (planet_num < MAX_PLANET) {
                    planet_list[planet_num - 1]->position = shootStart;
                    planet_list[planet_num++] = Create_Planet(waitPoint, (Vector2) { 0, 0 }, rand() % max_type + 1);
                }
                isFired = false;
            }
        }

        // 화면 업데이트 redraw
        if (redraw) {

            if (!paused) {
                background_timer += deltaTime;
                if (background_timer >= background_interval) {
                    current_scroll_frame = (current_scroll_frame + 1) % SCROLL_FRAMES;
                    background_timer = 0.0f;
                }
            }
            if (!display) {
                return 0;
            }
            
            //배경
            al_draw_scaled_bitmap(scroll_frames[current_scroll_frame],
                0, 0,
                al_get_bitmap_width(scroll_frames[current_scroll_frame]),
                al_get_bitmap_height(scroll_frames[current_scroll_frame]),
                0, 0,
                SCREEN_W, SCREEN_H,
                0);

            if (paused) {
                al_draw_text(pause_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTRE, "PAUSED");
            }

            //1.중력필드 배경, 2. 출발 지점, 3. 중심 순서로 그림
            al_draw_bitmap(gravityfield, center_x - 350, center_y - 350, 0);
            al_draw_bitmap(startpoint, init_x - 75, init_y - 75, 0);
            al_draw_bitmap(center, center_x - 15, center_y - 15, 0);
           
            // 다음 행성 표시
            al_draw_filled_rectangle(40, 40, 200, 200, al_map_rgb(100, 100, 100));
            al_draw_filled_rectangle(70, 70, 170, 170, al_map_rgb(50, 50, 50));
            al_draw_text(next_font, al_map_rgb(255, 255, 255), 120, 170, ALLEGRO_ALIGN_CENTER, "NEXT");

            // 점수 표시
            al_draw_filled_rectangle(40, 750, 200, 850, al_map_rgb(100, 100, 100));
            al_draw_filled_rectangle(50, 760, 190, 840, al_map_rgb(50, 50, 50));
            al_draw_text(score_text_font, al_map_rgb(255, 255, 255), 120, 850, ALLEGRO_ALIGN_CENTER, "SCORE");      
            al_draw_textf(score_best_font, al_map_rgb(255, 255, 255), 120, 815, ALLEGRO_ALIGN_CENTER, "BEST: %d", (score > high_score) ? score : high_score);
            al_draw_textf(score_font, al_map_rgb(255, 255, 255), 120, 770, ALLEGRO_ALIGN_CENTER, "%d", score);
            

            // 행성들 그리기
            for (int i = 0; i <= planet_num; ++i) {
                Planet* p = planet_list[i];
                if (!p) continue;
                ALLEGRO_BITMAP* planet_img = planet_images[p->type];

                if (planet_img) {
                    al_draw_rotated_bitmap(planet_img,
                        p->radius, p->radius,               // 이미지 중심 (x, y)
                        p->position.x, p->position.y,       // 그릴 위치
                        p->rotation,                        // 회전 각도 (라디안)
                        0);                                 // 플래그
                }
            }
            
            bool danger = false;
            for (int i = 0; i < planet_num; ++i) {
                Planet* p = planet_list[i];
                if (p && p->isLeavingGravityZone) {
                    danger = true;
                    break;
                }
            }

            //중력장 테두리 색상 표시
            if (danger) {
                al_draw_circle(center_x, center_y, 350, al_map_rgb(255, 0, 0), 3); // 붉은 테두리
            }
            else {
                al_draw_circle(center_x, center_y, 350, al_map_rgb(255, 255, 255), 1); // 기본 테두리
            }
            // 드래그 하는 동안, 유도선 그리기
            // 유도선 (중력 반영한 예측 궤도)
            if (isDragging) {
                Vector2 forceVec = Vector2_Clamp(Vector2_Scale(Vector2_Sub(dragEnd, dragStart), -line_length * 0.5f), maxForce);

                Vector2 position = shootStart;
                Vector2 velocity = forceVec;

                int dotCount = 12; //점 12개
                float predict_delta_time = 0.8f;   //점간의 간격 받아오는 시간 계산
                float gravityStrength = 1400000.0f;   //이 부분으로 휘는 정도 
                Vector2 gravityCenter = { center_x, center_y }; //중력 중간 위치

                for (int i = 0; i < dotCount; ++i) {  //포물선 계산
                    Vector2 toCenter = Vector2_Sub(gravityCenter, position);
                    float distance = Vector2_Length(toCenter);

                    if (distance == 0) break; // divide by zero 방지

                    Vector2 gravityDir = Vector2_Normalize(toCenter);
                    float gravityForce = gravityStrength / (distance * distance + 100.0f);  //100더한게 값 뜨는거 방지
                    Vector2 gravity = Vector2_Scale(gravityDir, gravityForce);


                    velocity = Vector2_Add(velocity, Vector2_Scale(gravity, predict_delta_time));
                    position = Vector2_Add(position, Vector2_Scale(velocity, predict_delta_time));

                    al_draw_filled_circle(position.x, position.y, 2, al_map_rgb(255, 255, 255));
                }
            }

            // 화면 갱신
            al_flip_display();
        }
    }

    // 해제
    reset_planets();
    destroy_game_resources(center, gravityfield, startpoint, scroll_frames, game_event_queue, timer);

    return;
}