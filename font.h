#pragma once
#include "init.h"

void init_all_fonts();
void destroy_all_fonts();
ALLEGRO_FONT* get_font(int size);

extern ALLEGRO_FONT* title_font;
extern ALLEGRO_FONT* menu_font;
extern ALLEGRO_FONT* score_font;
extern ALLEGRO_FONT* next_font;
extern ALLEGRO_FONT* score_text_font;
extern ALLEGRO_FONT* score_best_font;
extern ALLEGRO_FONT* username_font;
extern ALLEGRO_FONT* entername_font;
extern ALLEGRO_FONT* rank_font;
extern ALLEGRO_FONT* win_font;
extern ALLEGRO_FONT* pause_font;