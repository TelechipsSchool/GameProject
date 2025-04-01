#include "menu.h"

// 버튼 생성
Button menu_buttons[MENU_COUNT] = {
    {SCREEN_W / 2 - 100, 250, 200, 50},  // Start
    {SCREEN_W / 2 - 100, 320, 200, 50},  // Rank
    {SCREEN_W / 2 - 100, 390, 200, 50},   // Setting
    {SCREEN_W / 2 - 100, 460, 200, 50}   // Help
};

void menu() {
    // Allegro 초기화
    init();

    // Display 생성
   // ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);

    // 메뉴 화면 생성
    ALLEGRO_BITMAP* main_screen = load_bitmap_resized("images/menu.png", SCREEN_W, SCREEN_H);
    if (!main_screen) {
        DEBUG_MSG(main screen - 로드 실패);
        return 0;  // 또는 적절한 fallback 처리
    }

    // 폰트
    ALLEGRO_FONT* title_font = get_title_font();
    ALLEGRO_FONT* menu_font = get_menu_font();

    // 이벤트 큐 생성
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_draw_bitmap(main_screen, 0, 0, 0);

    MenuOption selected = MENU_START;
    bool running = true;

while (running) {
        ALLEGRO_EVENT event;

        // 키보드 입력 처리
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    selected = (selected - 1 + MENU_COUNT) % MENU_COUNT;
                    break;
                case ALLEGRO_KEY_DOWN:
                    selected = (selected + 1) % MENU_COUNT;
                    break;
                case ALLEGRO_KEY_ENTER:
                    if (selected == MENU_START) start_game();
                    else if (selected == MENU_RANK) display_ranks();
                    else if (selected == MENU_SETTING) printf("설정 화면\n");
                    else if (selected == MENU_HELP) {
                        ALLEGRO_BITMAP* help_screen = al_load_bitmap("images/help.png");
                        if (!help_screen) {
<<<<<<< HEAD
                            printf("help.png �ε� ����!\n");
=======
                            printf("help.png 로딩 실패!\n");
>>>>>>> 824479bc1251cfaefa43c975fc0811c0a3300ea6
                        }
                        else {
                            al_draw_scaled_bitmap(help_screen, 0, 0,
                                al_get_bitmap_width(help_screen), al_get_bitmap_height(help_screen),
                                0, 0, SCREEN_W, SCREEN_H, 0);
                            al_flip_display();
                        
<<<<<<< HEAD
                            // ESC�� ���� ������ ��ٸ��� ����
=======
                            // ESC가 눌릴 때까지 기다리는 루프
>>>>>>> 824479bc1251cfaefa43c975fc0811c0a3300ea6
                            bool in_help = true;
                            while (in_help) {
                                ALLEGRO_EVENT help_event;
                                al_wait_for_event(event_queue, &help_event);
                        
                                if (help_event.type == ALLEGRO_EVENT_KEY_DOWN &&
                                    help_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
<<<<<<< HEAD
                                    in_help = false; // ESC ������ ���� ����
=======
                                    in_help = false; // ESC 누르면 루프 종료
>>>>>>> 824479bc1251cfaefa43c975fc0811c0a3300ea6
                                }
                            }

                            al_destroy_bitmap(help_screen);
                        }
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    break;
                }
            }
        }

        // 화면 그리기
 
        if (main_screen!=NULL&& display!=NULL) {
            
            al_draw_scaled_bitmap(main_screen, 0, 0, al_get_bitmap_width(main_screen), al_get_bitmap_height(main_screen), 0, 0, SCREEN_W, SCREEN_H, 0);
        }
        else {
            DEBUG_MSG(main screen 또는 display - 이미지 로드 실패);
           // al_clear_to_color(al_map_rgb(20, 20, 20));
            return 0;
        }
        // 제목
        al_draw_text(title_font, al_map_rgb(181, 178, 255), SCREEN_W / 2, 300, ALLEGRO_ALIGN_CENTER, "SUPERNOVA");

        // 메뉴 옵션 출력 (선택된 메뉴는 노란색, 나머지는 흰색)
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
    al_destroy_font(menu_font);
    //al_destroy_display(display);
    al_destroy_display(display); display = NULL;
    al_destroy_event_queue(event_queue);
}
