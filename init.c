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
	al_install_keyboard,
	al_install_audio,
	al_init_acodec_addon

};



void init() {
	// Allegro �ʱ�ȭ
	al_init();

	for (int i = 0; i < SIZE(init_functions); ++i) {
		init_functions[i]();
	}
	if (!al_reserve_samples(16)) {
		DEBUG_MSG(reserve_samples - ���� �ޱ� ����);
		exit(1);
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
	menu_bgm= al_load_sample("audio/menu.ogg");
	al_play_sample(menu_bgm, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	if (!menu_bgm) {
		DEBUG_MSG(reserve_samples - ���� �ε� ����);
	}
}