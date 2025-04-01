#include "init.h"

ALLEGRO_DISPLAY* display = NULL;


void init() {
	// Allegro 초기화
	if (!al_init()) {
		printf("al_init() 실패\n");
		exit(1);
	}

	if (!al_init_image_addon()) {
		printf("init_image_addon() 실패\n");
		exit(1);
	}
	//al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

	// 입력 장치 활성화
	al_install_mouse();
	al_install_keyboard();

	// 난수 생성기를 초기화
	srand(time(NULL));

	if (!display) {
		display = al_create_display(SCREEN_W, SCREEN_H);
		if (!display) {
			printf("디스플레이 생성 실패\n");
			exit(1);
		}
	}
}