#pragma once
#include "init.h"
#include "font.h"
#include "display.h"
#include "audio.h"
#include "cosmic_protector.h"

// ���� �̸� �ִ� ����
#define MAX_NAME_LENGTH 20
void wait_or_skip(double duration);
void story1();
void story2(int score);
void story3();
void story4(char* username);
char* getUserName();