#pragma once
#include "init.h"
#include "font.h"
#include "display.h"
#include "audio.h"

// 유저 이름 최대 길이
#define MAX_NAME_LENGTH 20

void story1();
void story2();
void story3();
void story4(char* username);
char* getUserName();