#include "menu.h"

// �޴� ��ư
Button menu_buttons[MENU_COUNT] = {
    {SCREEN_W / 2 - 100, 250, 200, 50},  // Start
    {SCREEN_W / 2 - 100, 320, 200, 50},  // Rank
    {SCREEN_W / 2 - 100, 390, 200, 50},   // Setting
    {SCREEN_W / 2 - 100, 460, 200, 50}   // Help
};

void menu() {
    // �޴� ȭ�� ����
    ALLEGRO_BITMAP* main_screen = load_bitmap_resized("images/menu.png", SCREEN_W, SCREEN_H);
    if (!main_screen) {
        DEBUG_MSG(main screen - ȭ�� �ε� ����);
        return 0;  
    }

    ALLEGRO_SAMPLE* menu_bgm = al_load_sample("audio/menu.ogg");
    // ��Ʈ
     init_all_fonts();
    // �̺�Ʈ ť ����
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_draw_bitmap(main_screen, 0, 0, 0);
    al_play_sample(menu_bgm, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    MenuOption selected = MENU_START;
    bool running = true;

    while (running) {
        ALLEGRO_EVENT event;

        // Ű���� �Է� ó��
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    selected = (selected - 1 + MENU_COUNT) % MENU_COUNT;
                    play_music("audio/switch.ogg");
                    break;
                case ALLEGRO_KEY_DOWN:
                    selected = (selected + 1) % MENU_COUNT;
                    play_music("audio/switch.ogg");
                    break;
                case ALLEGRO_KEY_ENTER:
                    if (selected == MENU_START) { al_stop_samples();  al_destroy_sample(menu_bgm); menu_bgm = NULL; start_game(); }
                    else if (selected == MENU_RANK) display_ranks();
                    else if (selected == MENU_SETTING) {
                        //printf("���� ȭ��\n");
                        play_music("audio/enter.ogg");
                        show_setting_menu();

                        ALLEGRO_EVENT tmp;
                        while (al_get_next_event(event_queue, &tmp));
                    }
                    else if (selected == MENU_HELP) {
                        ALLEGRO_BITMAP* help_screen = al_load_bitmap("images/help.png");
                        play_music("audio/enter.ogg");
                        ALLEGRO_SAMPLE* sample = al_load_sample("audio/help.ogg");
                        ALLEGRO_SAMPLE_ID sample_id;

                        if (!sample) {
                            printf("���� �ε� ����!\n");
                        }
                        else {
                            al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &sample_id);


                        }
                        if (!help_screen) {

                            printf("help.png �ε� ����!\n");
                        }
                        else {
                            al_draw_scaled_bitmap(help_screen, 0, 0,
                                al_get_bitmap_width(help_screen), al_get_bitmap_height(help_screen),
                                0, 0, SCREEN_W, SCREEN_H, 0);
                            al_flip_display();


                            // ESC�� ���� ������ ��ٸ��� ����

                            bool in_help = true;
                            while (in_help) {
                                ALLEGRO_EVENT help_event;
                                al_wait_for_event(event_queue, &help_event);

                                if (help_event.type == ALLEGRO_EVENT_KEY_DOWN &&
                                    help_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                                    in_help = false; // ESC ������ ���� ����

                                    al_stop_sample(&sample_id);

                                }
                            }

                            al_destroy_bitmap(help_screen);
                            al_destroy_sample(sample);
                        }
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    play_music("audio/cancel.ogg");
                    break;
                }
            }
        }



        if (main_screen != NULL && display != NULL) {

            al_draw_scaled_bitmap(main_screen, 0, 0, al_get_bitmap_width(main_screen), al_get_bitmap_height(main_screen), 0, 0, SCREEN_W, SCREEN_H, 0);
        }
        else {
            DEBUG_MSG(main screen - �ε� ����);
            return 0;
        }
        // ȭ�� �׸���
        
        al_draw_text(title_font, al_map_rgb(181, 178, 255), SCREEN_W / 2, 300, ALLEGRO_ALIGN_CENTER, "SUPERNOVA");

        // �޴� �ɼ� ���(���õ� �޴��� �����, �������� ���)

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

// ���ҽ� ����
    al_destroy_bitmap(main_screen);
    al_destroy_display(display); display = NULL;
    al_destroy_sample(menu_bgm); menu_bgm = NULL;
    al_destroy_event_queue(event_queue);
    }

