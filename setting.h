#pragma once


#include "init.h"       // SCREEN_W, SCREEN_H, display �� ���ǵǾ�� ��
#include "font.h"         // get_menu_font() ��� �� �ʿ�
#include "display.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

typedef enum {//���̵� 3�ܰ�-�߰� ����Ʈ
    DIFFICULTY_EASY,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD
} Difficulty;


extern Difficulty current_difficulty;

void show_setting_menu(void);
