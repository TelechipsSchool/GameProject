#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <stdlib.h>
#include "init.h"
#include "menu.h"
#include "display.h"
#include "vector.h"
#include "rank.h"
#include "setting.h"
#include "font.h"
#include "audio.h"
#include "story.h"
#include "image.h"
#include "resource.h"
#include "planet.h"
#include "physics.h"

// ������ ����
float line_length = 0.15f;

// �༺ �߻� ��ġ
#define init_x 300
#define init_y SCREEN_H / 2


//��� ���� Ÿ�̸�
float background_timer = 0.0f;
float background_interval = 0.3f;

// �༺ ��� ��ġ
#define wait_x 120
#define wait_y 120

// ��ǥ �༺ ��ġ
#define center_x SCREEN_W - 400
#define center_y SCREEN_H / 2

// ���� ��� (���� �� �� ���ϰ� �ϱ�)
double centerCoefficient = 0.03f;
double RESTITUTION = 0.3f;      // ź�� ���
double dragForce = 150.0f;      // ���� ��
double maxForce = 200.0f;      // �ִ� ���� ��

void start_game(char* username);