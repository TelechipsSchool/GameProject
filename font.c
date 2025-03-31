#include "font.h"

ALLEGRO_FONT* get_title_font() {
	ALLEGRO_FONT* font = al_load_font("fonts/D2Coding/D2CodingBold-Ver1.3.2-20180524.ttf", 100, 0);
	return font;
}

ALLEGRO_FONT* get_menu_font() {
	// ÆùÆ®
	ALLEGRO_FONT* font = al_load_font("fonts/D2Coding/D2CodingBold-Ver1.3.2-20180524.ttf", 30, 0);
	return font;
}