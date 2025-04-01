#pragma once


#include "init.h"       // SCREEN_W, SCREEN_H, display 등 정의되어야 함
#include "font.h"         // get_menu_font() 사용 시 필요
#include "display.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

typedef enum {//난이도 3단계-중간 디폴트
    DIFFICULTY_EASY,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD
} Difficulty;


extern Difficulty current_difficulty;

void show_setting_menu(void);
