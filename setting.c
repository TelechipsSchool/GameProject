#include "setting.h"

Difficulty current_difficulty = DIFFICULTY_MEDIUM;//����Ʈ �߰����̵�

void show_setting_menu() {


    ALLEGRO_BITMAP* setting_screen = load_bitmap_resized("images/setting.png", SCREEN_W, SCREEN_H);
    if (!setting_screen) {
        DEBUG_MSG(setting screen - �̹��� �ε� ����);
        return;
    }
    //��Ʈ
    ALLEGRO_FONT* title_font = get_title_font();
    ALLEGRO_FONT* menu_font = get_menu_font();

    //���� �� �̺�Ʈ ť ����
    ALLEGRO_EVENT_QUEUE* setting_queue = al_create_event_queue();
    al_register_event_source(setting_queue, al_get_keyboard_event_source());
    al_register_event_source(setting_queue, al_get_display_event_source(display));

    al_draw_bitmap(setting_screen, 0, 0, 0);
    const char* options[] = { "EASY", "MEDIUM", "HARD" };
    int selected = (int)current_difficulty;//enum���� ����
    bool in_setting = true;

    while (in_setting) {


        ALLEGRO_EVENT ev;
        al_wait_for_event(setting_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                selected = (selected + 2) % 3;//��-> ���̵���
                break;
            case ALLEGRO_KEY_DOWN:
                selected = (selected + 1) % 3;//�Ʒ�����->���̵���
                break;
            case ALLEGRO_KEY_ENTER:
                current_difficulty = (Difficulty)selected;//����Ʈ �߰����̵�
                in_setting = false;
                break;
            case ALLEGRO_KEY_ESCAPE:
                in_setting = false;
                break;
            }
        }


        if (setting_screen != NULL && display != NULL) {

            al_draw_scaled_bitmap(setting_screen, 0, 0, al_get_bitmap_width(setting_screen), al_get_bitmap_height(setting_screen), 0, 0, SCREEN_W, SCREEN_H, 0);
        }
        else {
            DEBUG_MSG(setting screen �Ǵ� display - �ε� ����);
            // al_clear_to_color(al_map_rgb(20, 20, 20));
            return 0;
        }
        // ȭ�� �׸���

        al_draw_text(title_font, al_map_rgb(255, 100, 255), SCREEN_W / 2, 200, ALLEGRO_ALIGN_CENTER, "SELECT DIFFICULTY");

        for (int i = 0; i < 3; ++i) {
            ALLEGRO_COLOR color = (i == selected) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255);
            al_draw_text(menu_font, color, SCREEN_W / 2, 400 + i * 65, ALLEGRO_ALIGN_CENTER, options[i]);
        }
        al_flip_display();
    }

    al_destroy_bitmap(setting_screen);
    al_destroy_font(menu_font);
    al_destroy_event_queue(setting_queue);
    
}
