#pragma once
#include "init.h"
#include "font.h"
#include "display.h"
#include "audio.h"

// ���� �̸� �ִ� ����
#define MAX_NAME_LENGTH 20
void wait_or_skip(double duration);
void story1();
void story2();
void story3();
void story4(char* username);
void story5();
char* getUserName();