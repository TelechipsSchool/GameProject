#include "init.h"

ALLEGRO_DISPLAY* display = NULL;


void init() {
	// Allegro �ʱ�ȭ
	if (!al_init()) {
		printf("al_init() ����\n");
		exit(1);
	}

	if (!al_init_image_addon()) {
		printf("init_image_addon() ����\n");
		exit(1);
	}
	//al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

	// �Է� ��ġ Ȱ��ȭ
	al_install_mouse();
	al_install_keyboard();

	// ���� �����⸦ �ʱ�ȭ
	srand(time(NULL));

	if (!display) {
		display = al_create_display(SCREEN_W, SCREEN_H);
		if (!display) {
			printf("���÷��� ���� ����\n");
			exit(1);
		}
	}
}