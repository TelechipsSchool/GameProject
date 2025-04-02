#pragma once
#include "init.h"

//ALLEGRO_FONT* get_title_font();
//ALLEGRO_FONT* get_menu_font();
//ALLEGRO_FONT* get_next_font();
//ALLEGRO_FONT* get_score_font();
//ALLEGRO_FONT* get_score_text_font();
//ALLEGRO_FONT* get_score_best_font();
//ALLEGRO_FONT* get_entername_font();
//ALLEGRO_FONT* get_username_font();
//ALLEGRO_FONT* get_win_font();


//ALLEGRO_FONT* get_font(int size);
void destroy_all_fonts();
void init_all_fonts();

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