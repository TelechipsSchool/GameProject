#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <allegro5/allegro.h>
ALLEGRO_BITMAP* planet_images[8];
void load_planet_images();
void destroy_planet_images(ALLEGRO_BITMAP** planet_images);