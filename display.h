#pragma once
#include "init.h"

extern ALLEGRO_DISPLAY* display;
ALLEGRO_BITMAP* load_bitmap_resized(const char* filename, int w, int h);