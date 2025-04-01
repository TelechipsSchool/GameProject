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
	// Allegro 초기화
	al_init();

	for (int i = 0; i < SIZE(init_functions); ++i) {
		init_functions[i]();
	}


	// 난수 생성기를 초기화
	srand(time(NULL));

	if (!display) {//게임 중 ESC 종료시 출력 
		display = al_create_display(SCREEN_W, SCREEN_H);
		if (!display) {
			DEBUG_MSG(create_display - 이미지 생성 실패);
			exit(1);
		}
	}
}