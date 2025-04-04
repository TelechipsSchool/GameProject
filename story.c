#include "story.h"
bool skip_story = false;

void wait_or_skip(double duration) {
    double start = al_get_time();
    while (al_get_time() - start < duration) {
        ALLEGRO_KEYBOARD_STATE key_state;
        al_get_keyboard_state(&key_state);
        if (al_key_down(&key_state, ALLEGRO_KEY_Q)) {
            skip_story = true;
            break;
        }
        al_rest(0.01);
    }
}

void story1() {
    // 게임 시작 시 스토리

    // 배경 화면 생성
    ALLEGRO_BITMAP* story_screen = load_bitmap_resized("images/setting.png", SCREEN_W, SCREEN_H);
    if (!story_screen || !display) {
        DEBUG_MSG(story_screen - 화면 로드 실패);
        return;
    }
    al_draw_scaled_bitmap(story_screen, 0, 0, al_get_bitmap_width(story_screen), al_get_bitmap_height(story_screen), 0, 0, SCREEN_W, SCREEN_H, 0);
    
    // 캐릭터 생성
    ALLEGRO_BITMAP* a_play = load_bitmap_resized("images/astronaut/play.png", 300, 300);
    if (!a_play) {
        DEBUG_MSG(a_play - 화면 로드 실패);
        return;
    }
    ALLEGRO_BITMAP* a_holdingplanet = load_bitmap_resized("images/astronaut/holdingplanet.png", 300, 300);
    if (!a_holdingplanet) {
        DEBUG_MSG(a_holdingplanet - 화면 로드 실패);
        return;
    }

    int y_position = 100;

    // 텍스트 시작
    al_rest(2.0);
    al_draw_bitmap(a_play, 1200, 400, 0);
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"안녕, 난 암스트롱이야");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"너의 이름은 뭐야?");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 100, ALLEGRO_ALIGN_CENTER, u8"<<   Enter를 눌러서 다음으로 이동  >>");
    al_flip_display();

    // Enter 키를 기다리는 루프 -> 이름 입력 창으로 이동
    ALLEGRO_KEYBOARD_STATE key_state;
    bool enter_pressed = false;

    while (!enter_pressed) {
        al_get_keyboard_state(&key_state);
        if (al_key_down(&key_state, ALLEGRO_KEY_ENTER)) {
            play_music("audio/enter.ogg");
            enter_pressed = true;
        }
        al_flip_display();
    }
    char* username = getUserName();

    al_draw_scaled_bitmap(story_screen, 0, 0, al_get_bitmap_width(story_screen), al_get_bitmap_height(story_screen), 0, 0, SCREEN_W, SCREEN_H, 0);

    al_draw_bitmap(a_holdingplanet, 1300, 600, 0);

    // 텍스트 위에서부터 다시 시작
    y_position = 100;
    wait_or_skip(2.0);
    al_draw_textf(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"반가워 %s!", username);
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"지구가 점점 온난화로 인해 살기 힘든 환경이 되어가고 있어.");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"우리는 이제 새로운 집을 찾아 떠나야 해.");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"다행히도, 다른 은하계에서 살 수 있는 행성을 찾았어.");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"하지만 그곳엔 태양이 없어서... 우리가 태양을 만들어야 해!");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"우리의 첫 번째 목표는, 주변 행성들을 융합해서 태양을 만드는 거야.");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"태양이 없으면 아무것도 할 수 없으니까!");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 100, ALLEGRO_ALIGN_CENTER, u8"<<   Enter를 눌러서 게임 시작  >>");
    al_flip_display();

    enter_pressed = false;
    while (!enter_pressed) {
        al_get_keyboard_state(&key_state);
        if (al_key_down(&key_state, ALLEGRO_KEY_ENTER)) {
            play_music("audio/enter.ogg");
            enter_pressed = true;
        }
        al_flip_display();
    }

    // 게임 진행
    game1(username);
}

void story2() {
    // 소행성 게임 시작 전 스토리

    // 배경 화면 생성
    ALLEGRO_BITMAP* story_screen = load_bitmap_resized("images/setting.png", SCREEN_W, SCREEN_H);
    if (!story_screen) {
        DEBUG_MSG(story_screen - 화면 로드 실패);
        return 0;
    }
    // 캐릭터 생성
    ALLEGRO_BITMAP* a_flight = load_bitmap_resized("images/astronaut/flight.png", 300, 300);
    if (!a_flight) {
        DEBUG_MSG(a_flight - 화면 로드 실패);
        return 0;
    }
    al_draw_scaled_bitmap(story_screen, 0, 0, al_get_bitmap_width(story_screen), al_get_bitmap_height(story_screen), 0, 0, SCREEN_W, SCREEN_H, 0);

    int y_position = 100;

    // 텍스트 시작
    al_rest(2.0);
    al_draw_bitmap(a_flight, 1200, 400, 0);
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"좋아! 드디어 태양을 만들었어.");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"이제 우리는 그 행성에서 살아갈 수 있어!");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"하지만, 문제가 있어. 그 행성으로 소행성이 다가오고 있어.");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"우리는 그 소행성을 막아야 해!");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"화살표로 비행기를 조종하면서, Space Bar를 눌러 총을 발사할 수 있어.");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"소행성과 충돌하면 생명이 줄어드니 조심해!");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 100, ALLEGRO_ALIGN_CENTER, u8"<<   Enter를 눌러서 게임 시작  >>");
    al_flip_display();

    ALLEGRO_KEYBOARD_STATE key_state;
    bool enter_pressed = false;
    while (!enter_pressed) {
        al_get_keyboard_state(&key_state);
        if (al_key_down(&key_state, ALLEGRO_KEY_ENTER)) {
            play_music("audio/enter.ogg");
            enter_pressed = true;
        }
        al_flip_display();
    }
}

void story3() {
    // 외계인 출몰 시 스토리

    // 배경 화면 생성
    ALLEGRO_BITMAP* story_screen = load_bitmap_resized("images/setting.png", SCREEN_W, SCREEN_H);
    if (!story_screen) {
        DEBUG_MSG(story_screen - 화면 로드 실패);
        return 0;
    }
    // 캐릭터 생성
    ALLEGRO_BITMAP* a_weapon = load_bitmap_resized("images/astronaut/weapon.png", 300, 300);
    if (!a_weapon) {
        DEBUG_MSG(a_weapon - 화면 로드 실패);
        return 0;
    }
    al_draw_scaled_bitmap(story_screen, 0, 0, al_get_bitmap_width(story_screen), al_get_bitmap_height(story_screen), 0, 0, SCREEN_W, SCREEN_H, 0);

    int y_position = 100;

    // 텍스트 시작
    al_rest(2.0);
    al_draw_bitmap(a_weapon, 1200, 400, 0);
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"뭐야? 외계인들이 나타났어!");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"우리가 행성을 지키는 걸 방해하려고 하는 놈들이야.");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"외계인들은 우주선을 타고 움직이며 총알을 발사하니 조심해!");
    y_position += 100;
    //al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 100, ALLEGRO_ALIGN_CENTER, u8"<<   Enter를 눌러서 게임 재개  >>");
    al_flip_display();

    ALLEGRO_KEYBOARD_STATE key_state;
    bool enter_pressed = false;
    while (!enter_pressed) {
        al_get_keyboard_state(&key_state);
        if (al_key_down(&key_state, ALLEGRO_KEY_ENTER)) {
            play_music("audio/enter.ogg");
            enter_pressed = true;
        }
        al_flip_display();
    }

    // 다음 게임 실행
}

void story4(char* username) {
    // 게임 승리 시 스토리

    // 배경 화면 생성
    ALLEGRO_BITMAP* story_screen = load_bitmap_resized("images/setting.png", SCREEN_W, SCREEN_H);
    if (!story_screen) {
        DEBUG_MSG(story_screen - 화면 로드 실패);
        return 0;
    }
    // 캐릭터 생성
    ALLEGRO_BITMAP* a_rocket = load_bitmap_resized("images/astronaut/rocket.png", 300, 300);
    if (!a_rocket) {
        DEBUG_MSG(a_rocket - 화면 로드 실패);
        return 0;
    }
    ALLEGRO_BITMAP* a_end = load_bitmap_resized("images/astronaut/end.png", 600, 600);
    if (!a_rocket) {
        DEBUG_MSG(a_end - 화면 로드 실패);
        return 0;
    }
    al_draw_scaled_bitmap(story_screen, 0, 0, al_get_bitmap_width(story_screen), al_get_bitmap_height(story_screen), 0, 0, SCREEN_W, SCREEN_H, 0);

    int y_position = 100;

    // 텍스트 시작
    al_rest(2.0);
    al_draw_bitmap(a_rocket, 1200, 400, 0);
    al_draw_textf(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"해냈어, %s! 소행성도, 외계 적들도 모두 물리쳤어!", username);
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"이제 우리는 그 행성에서 살아갈 수 있어!");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"이제 이곳은 우리가 살아갈 새로운 고향이 될 거야.");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"지구에서 멀리 떠나왔지만, 인류는 다시 한번 희망을 찾았어.");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_textf(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"넌 정말 대단해, %s! 너 없었으면 우린 이겨내지 못했을 거야.", username);
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"자, 이제 새로운 시대를 시작해볼까? 우리 모두의 새로운 미래를 위해!");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    al_rest(5.0);

    al_draw_scaled_bitmap(story_screen, 0, 0, al_get_bitmap_width(story_screen), al_get_bitmap_height(story_screen), 0, 0, SCREEN_W, SCREEN_H, 0);

    // 텍스트 위에서부터 다시 시작
    al_draw_bitmap(a_end, SCREEN_W / 2 - 300, SCREEN_H - 800, 0);
    al_draw_textf(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 200, ALLEGRO_ALIGN_CENTER, u8"넌 정말 대단해, %s! 너 없었으면 우린 이겨내지 못했을 거야.", username);
    play_music("audio/story.ogg");
    al_flip_display();
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 100, ALLEGRO_ALIGN_CENTER, u8"<<   Enter를 눌러서 게임 종료  >>");
    al_flip_display();

    ALLEGRO_KEYBOARD_STATE key_state;
    bool enter_pressed = false;
    while (!enter_pressed) {
        al_get_keyboard_state(&key_state);
        if (al_key_down(&key_state, ALLEGRO_KEY_ENTER)) {
            play_music("audio/enter.ogg");
            enter_pressed = true;
        }
        al_flip_display();
    }
}

void story5() {
    // 패배 시 스토리

    // 배경 화면 생성
    ALLEGRO_BITMAP* story_screen = load_bitmap_resized("images/setting.png", SCREEN_W, SCREEN_H);
    if (!story_screen) {
        DEBUG_MSG(story_screen - 화면 로드 실패);
        return 0;
    }
    // 캐릭터 생성
    ALLEGRO_BITMAP* a_defeat = load_bitmap_resized("images/astronaut/defeat.png", 300, 300);
    if (!a_defeat) {
        DEBUG_MSG(a_defeat - 화면 로드 실패);
        return 0;
    }
    al_draw_scaled_bitmap(story_screen, 0, 0, al_get_bitmap_width(story_screen), al_get_bitmap_height(story_screen), 0, 0, SCREEN_W, SCREEN_H, 0);

    int y_position = 100;

    // 텍스트 시작
    al_rest(2.0);
    al_draw_bitmap(a_defeat, 1200, 400, 0);
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"안 돼... 결국 우리가 실패하고 말았어.");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"소행성도, 외계인도... 우리가 감당하기엔 너무 강했어.");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"이번엔 졌지만, 다음엔 반드시 승리할 거야!");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"다시 도전할 준비가 됐을 때 돌아와 줘!");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    wait_or_skip(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 100, ALLEGRO_ALIGN_CENTER, u8"<<   Enter를 눌러서 메뉴로 돌아가기   >>");
    al_flip_display();

    ALLEGRO_KEYBOARD_STATE key_state;
    bool enter_pressed = false;
    while (!enter_pressed) {
        al_get_keyboard_state(&key_state);
        if (al_key_down(&key_state, ALLEGRO_KEY_ENTER)) {
            play_music("audio/enter.ogg");
            enter_pressed = true;
        }
        al_flip_display();
    }
}

char* getUserName() {
    char* username = (char*)malloc(MAX_NAME_LENGTH);
    if (!username) {
        DEBUG_MSG(username - 메모리 할당 실패);
        return NULL;
    }
    username[0] = '\0';  // 초기화

    int username_length = 0;

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);  // 60fps
    if (!event_queue || !timer || !display) {
        DEBUG_MSG(event queue or timer or display - 생성 실패);
        free(username);  // 메모리 누수 방지
        return NULL;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    bool typing = true;
    while (typing) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                typing = false;  // ESC 키로 종료
                play_music("audio/cancel.ogg");
                menu();
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && username_length > 0) {
                play_music("audio/erase.ogg");
                username_length--;
                username[username_length] = '\0';
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER && username_length > 0) {
                play_music("audio/enter.ogg");
                typing = false;
            }
            else if (ev.keyboard.keycode >= ALLEGRO_KEY_A && ev.keyboard.keycode <= ALLEGRO_KEY_Z) {
                if (username_length < MAX_NAME_LENGTH - 1) {
                    play_music("audio/keyboard.ogg");
                    username[username_length] = ev.keyboard.keycode - ALLEGRO_KEY_A + 'A';
                    username_length++;
                    username[username_length] = '\0';  // 문자열 끝 처리
                }
            }
        }

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(entername_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 4, ALLEGRO_ALIGN_CENTER, "Enter your name");
            al_draw_rectangle(SCREEN_W / 2 - 200, SCREEN_H / 2 + 10, SCREEN_W / 2 + 200, SCREEN_H / 2 + 70, al_map_rgb(255, 255, 255), 2);
            al_draw_text(username_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 25, ALLEGRO_ALIGN_CENTER, username);
            al_flip_display();
        }
    }

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);

    return username;  // 동적 할당된 메모리를 반환
}