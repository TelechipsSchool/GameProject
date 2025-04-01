#include "init.h"

#define SIZE(a) (sizeof(a)/sizeof((a)[0]))

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_SAMPLE* menu_bgm = NULL;


void (*init_functions[])(void) = {
	al_init_image_addon,
	al_init_font_addon,
	al_init_ttf_addon,
	al_init_primitives_addon,
	al_install_mouse,
	al_install_keyboard

};



void init() {
	// Allegro �ʱ�ȭ
	al_init();

	for (int i = 0; i < SIZE(init_functions); ++i) {
		init_functions[i]();
	}


	// ���� �����⸦ �ʱ�ȭ
	srand(time(NULL));

	if (!display) {//���� �� ESC ����� ��� 
		display = al_create_display(SCREEN_W, SCREEN_H);
		if (!display) {
			DEBUG_MSG(create_display - �̹��� ���� ����);
			exit(1);
		}
	}
}