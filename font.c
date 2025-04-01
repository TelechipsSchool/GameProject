#include "font.h"

#define FONT_PATH "fonts/ChunkFive-Regular.otf"

ALLEGRO_FONT* get_title_font() {
	ALLEGRO_FONT* font = al_load_font(FONT_PATH, 140, 0);
	return font;
}

ALLEGRO_FONT* get_menu_font() {
	// 폰트
	ALLEGRO_FONT* font = al_load_font(FONT_PATH, 50, 0);
	return font;
}

ALLEGRO_FONT* get_next_font() {
	// 폰트
	ALLEGRO_FONT* font = al_load_font(FONT_PATH, 30, 0);
	return font;
}

ALLEGRO_FONT* get_score_text_font() {
	// 폰트
	ALLEGRO_FONT* font = al_load_font(FONT_PATH, 30, 0);
	return font;
}

ALLEGRO_FONT* get_score_font() {
	// 폰트
	ALLEGRO_FONT* font = al_load_font(FONT_PATH, 40, 0);
	return font;
}

ALLEGRO_FONT* get_score_best_font() {
	// 폰트
	ALLEGRO_FONT* font = al_load_font(FONT_PATH, 20, 0);
	return font;
}

ALLEGRO_FONT* get_username_font() {
	// 폰트
	ALLEGRO_FONT* font = al_load_font(FONT_PATH, 30, 0);
	return font;
}

ALLEGRO_FONT* get_entername_font() {
	// 폰트
	ALLEGRO_FONT* font = al_load_font(FONT_PATH, 50, 0);
	return font;
}

ALLEGRO_FONT* get_rank_font() {
	// 폰트
	ALLEGRO_FONT* font = al_load_font(FONT_PATH, 50, 0);
	return font;
}

ALLEGRO_FONT* get_win_font() {
	// 폰트
	ALLEGRO_FONT* font = al_load_font(FONT_PATH, 50, 0);
	return font;
}