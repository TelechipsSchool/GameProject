#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "init.h"
#include "display.h"
#include "audio.h"

//¹è°æ
#define SCROLL_FRAMES 40

void init_game_resources(ALLEGRO_TIMER** timer, ALLEGRO_EVENT_QUEUE** queue);
void load_game_sounds(ALLEGRO_SAMPLE** pull, ALLEGRO_SAMPLE** release);
void load_scroll_background(ALLEGRO_BITMAP* scroll_frames[]);