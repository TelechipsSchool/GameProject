#pragma once
#include "init.h"
#include "display.h"
#include "font.h"

typedef enum { MENU_START, MENU_RANK, MENU_SETTING, MENU_HELP, MENU_COUNT } MenuOption;

typedef struct {
    int x, y, width, height;
} Button;

void menu();