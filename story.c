#include "story.h"

void story1() {
    // 메뉴 화면 생성
    ALLEGRO_BITMAP* story_screen = load_bitmap_resized("images/setting.png", SCREEN_W, SCREEN_H);
    if (!story_screen) {
        DEBUG_MSG(story_screen - 화면 로드 실패);
        return 0;
    }
    al_draw_scaled_bitmap(story_screen, 0, 0, al_get_bitmap_width(story_screen), al_get_bitmap_height(story_screen), 0, 0, SCREEN_W, SCREEN_H, 0);

    int y_position = 100;

    // 텍스트 시작
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"안녕, 난 암스트롱이야");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    al_rest(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"너의 이름은 뭐야?");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    al_rest(2.0);

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

    // 텍스트 위에서부터 다시 시작
    y_position = 100;

    al_draw_textf(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"반가워 %s!", username);
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    al_rest(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"지구가 점점 온난화로 인해 살기 힘든 환경이 되어가고 있어.");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"우리는 이제 새로운 집을 찾아 떠나야 해.");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    al_rest(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"다행히도, 다른 은하계에서 살 수 있는 행성을 찾았어.");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"하지만 그곳엔 태양이 없어서... 우리가 태양을 만들어야 해!");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    al_rest(2.0);

    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"우리의 첫 번째 목표는, 주변 행성들을 융합해서 태양을 만드는 거야.");
    y_position += 100;
    al_draw_text(story_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y_position, ALLEGRO_ALIGN_CENTER, u8"태양이 없으면 아무것도 할 수 없으니까!");
    play_music("audio/story.ogg");
    al_flip_display();
    y_position += 100;
    al_rest(2.0);

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
	start_game(username);
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
                    //play_music("audio/keyboard.ogg");
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