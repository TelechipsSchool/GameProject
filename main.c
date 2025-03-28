#define _CRT_SECURE_NO_WARNINGS

#include "init.h"
#include "display.h"
#include "button.h"
#include "font.h"

int main(void) {

	// Allegro �ʱ�ȭ
    init();
    
	// Display ����
	ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);

    // �޴� ȭ�� ����
    ALLEGRO_BITMAP* main_screen = load_bitmap_resized("images/menu.png", SCREEN_W, SCREEN_H);

	// ��Ʈ
    ALLEGRO_FONT* font = get_font();

	// �̺�Ʈ ť ����
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_draw_bitmap(main_screen, 0, 0, 0);

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
                    break;
                case ALLEGRO_KEY_DOWN:
                    selected = (selected + 1) % MENU_COUNT;
                    break;
                case ALLEGRO_KEY_ENTER:
                    if (selected == MENU_START) start_game();
                    else if (selected == MENU_RANK) printf("��ŷ ����\n");
                    else if (selected == MENU_SETTING) printf("���� ȭ��\n");
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    break;
                }
            }
        }

        // ȭ�� �׸���
        al_draw_scaled_bitmap(main_screen, 0, 0, al_get_bitmap_width(main_screen), al_get_bitmap_height(main_screen), 0, 0, SCREEN_W, SCREEN_H, 0);

        // �޴� �ɼ� ��� (���õ� �޴��� �����, �������� ���)
        al_draw_text(font, (selected == MENU_START) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255),
            SCREEN_W / 2, 250, ALLEGRO_ALIGN_CENTER, "START");
        al_draw_text(font, (selected == MENU_RANK) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255),
            SCREEN_W / 2, 320, ALLEGRO_ALIGN_CENTER, "RANK");
        al_draw_text(font, (selected == MENU_SETTING) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255),
            SCREEN_W / 2, 390, ALLEGRO_ALIGN_CENTER, "SETTING");

        al_flip_display();
    }

    // ���ҽ� ����
    al_destroy_bitmap(main_screen);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}