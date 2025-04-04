#include "menu.h"

// 메뉴 버튼
Button menu_buttons[MENU_COUNT] = {
    {SCREEN_W / 2 - 100, 250, 200, 50},  // Start
    {SCREEN_W / 2 - 100, 320, 200, 50},  // Rank
    {SCREEN_W / 2 - 100, 390, 200, 50},   // Setting
    {SCREEN_W / 2 - 100, 460, 200, 50}   // Help
};

void menu() {
    // 이미지 포인터 선언
    ALLEGRO_BITMAP* main_screen = NULL, * title = NULL;
    ALLEGRO_BITMAP* a_hi = NULL, * a_laptop = NULL, * a_superman = NULL;

    // 이미지 정보 배열
    ImageAsset assets[] = {
        {"images/menu.png", SCREEN_W, SCREEN_H, &main_screen},
        {"images/title.png", 600, 120, &title},
        {"images/astronaut/hi.png", 100, 100, &a_hi},
        {"images/astronaut/laptop.png", 200, 200, &a_laptop},
        {"images/astronaut/superman.png", 200, 200, &a_superman}
    };

    // 이미지 로딩
    if (!load_image_assets(assets, sizeof(assets) / sizeof(assets[0]))) {
        return; // 하나라도 로드 실패 시 조기 종료
    }

    ALLEGRO_SAMPLE_ID menu_bgm_id;
    ALLEGRO_SAMPLE* menu_bgm = al_load_sample("audio/menu.ogg");
    // 폰트
    init_all_fonts();
    // 이벤트 큐 생성
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    
    al_draw_bitmap(main_screen, 0, 0, 0);
    
    al_play_sample(menu_bgm, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    MenuOption selected = MENU_START;
    bool running = true;

    while (running) {
        ALLEGRO_EVENT event;

        // 키보드 입력 처리
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    play_music("audio/switch.ogg");
                    selected = (selected - 1 + MENU_COUNT) % MENU_COUNT;                    
                    break;
                case ALLEGRO_KEY_DOWN:
                    play_music("audio/switch.ogg");
                    selected = (selected + 1) % MENU_COUNT;
                    break;
                case ALLEGRO_KEY_ENTER:
                    al_stop_samples();
                    play_music("audio/enter.ogg");
                    if (selected == MENU_START) {
                        game2("", 0, 9102);
                    }
                    else if (selected == MENU_RANK) display_ranks();
                    else if (selected == MENU_SETTING) {
                        show_setting_menu();

                        ALLEGRO_EVENT tmp;
                        while (al_get_next_event(event_queue, &tmp));
                    }
                    else if (selected == MENU_HELP) {
                        help_menu();
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    play_music("audio/cancel.ogg");
                    
                   // return;
                    break;
                }
            }
        }

        if (main_screen != NULL && display != NULL) {

            al_draw_scaled_bitmap(main_screen, 0, 0, al_get_bitmap_width(main_screen), al_get_bitmap_height(main_screen), 0, 0, SCREEN_W, SCREEN_H, 0);
        }
        else {
            DEBUG_MSG(main screen - 로드 실패);
            return 0;
        }
        // 화면 그리기
        al_draw_bitmap(a_laptop, 1100, 300, 0);
        al_draw_bitmap(a_superman, 400, 100, 0);
        al_draw_bitmap(a_hi, SCREEN_W / 2 - 200, 525 + 70 * selected, 0);
        
        /*al_draw_text(title_font, al_map_rgb(250, 237, 125), SCREEN_W / 2, 300, ALLEGRO_ALIGN_CENTER, "SUPERNOVA");*/
        al_draw_bitmap(title, SCREEN_W / 2 - 300, 300, 0);

        // 메뉴 옵션 출력(선택된 메뉴는 노란색, 나머지는 흰색)
        al_draw_text(menu_font, (selected == MENU_START) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255),
            SCREEN_W / 2, 550, ALLEGRO_ALIGN_CENTER, "START");
        al_draw_text(menu_font, (selected == MENU_RANK) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255),
            SCREEN_W / 2, 620, ALLEGRO_ALIGN_CENTER, "RANK");
        al_draw_text(menu_font, (selected == MENU_SETTING) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255),
            SCREEN_W / 2, 690, ALLEGRO_ALIGN_CENTER, "SETTING");
        al_draw_text(menu_font, (selected == MENU_HELP) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255),
            SCREEN_W / 2, 780, ALLEGRO_ALIGN_CENTER, "HELP");
        
        al_flip_display();
    }

// 리소스 해제
    al_destroy_bitmap(main_screen);
    al_destroy_bitmap(a_hi);
    al_destroy_display(display); display = NULL;
    al_destroy_sample(menu_bgm); menu_bgm = NULL;
    al_destroy_event_queue(event_queue);    
}