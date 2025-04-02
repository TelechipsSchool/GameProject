#include "font.h"

#define FONT_PATH "fonts/ChunkFive-Regular.otf"
static ALLEGRO_FONT* fonts[256] = { NULL };

// 전역 폰트 포인터
ALLEGRO_FONT* title_font = NULL;
ALLEGRO_FONT* menu_font = NULL;
ALLEGRO_FONT* score_font = NULL;
ALLEGRO_FONT* next_font = NULL;
ALLEGRO_FONT* score_text_font = NULL;
ALLEGRO_FONT* score_best_font = NULL;
ALLEGRO_FONT* username_font = NULL;
ALLEGRO_FONT* entername_font = NULL;
ALLEGRO_FONT* rank_font = NULL;
ALLEGRO_FONT* win_font = NULL;
ALLEGRO_FONT* pause_font = NULL;

ALLEGRO_FONT* get_font(int size) {
    if (size <= 0 || size >= 256) return NULL;
    if (!fonts[size]) {
        fonts[size] = al_load_font(FONT_PATH, size, 0);
        if (!fonts[size]) {
            printf("폰트 로딩 실패: %d\n", size);
			exit(1);
        }
    }
    return fonts[size];
}

void destroy_all_fonts() {
    for (int i = 0; i < 256; ++i) {
        if (fonts[i]) {
            al_destroy_font(fonts[i]);
            fonts[i] = NULL;
        }
    }
}



// 초기화 함수
void init_all_fonts() {
	title_font = get_font(140);
	menu_font = get_font(50);
	next_font = get_font(30);
	score_font = get_font(40);
	score_text_font = get_font(30);
	score_best_font = get_font(20);
	username_font = get_font(30);
	entername_font = get_font(50);
	rank_font = get_font(50);
	win_font = get_font(50);
	pause_font = get_font(50);
}