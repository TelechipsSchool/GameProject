#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "init.h"
#include "menu.h"
#include "display.h"
#include "rank.h"
#include "setting.h"
#include "font.h"
#include "audio.h"
#include "story.h"
#include "image.h"
#include "resource.h"
#include "planet.h"
#include "physics.h"

//��� ���� Ÿ�̸�
float background_timer = 0.0f;
const float background_interval = 0.3f;

// �༺ �߻� ��ġ
#define init_x 300
#define init_y SCREEN_H / 2

// �༺ ��� ��ġ
#define wait_x 120
#define wait_y 120

// �߻� ����
Vector2 shootStart = { init_x, init_y };
Vector2 waitPoint = { wait_x, wait_y };

// ��ǥ �༺ ��ġ
#define center_x SCREEN_W - 400
#define center_y SCREEN_H / 2
const Vector2 gravityCenter = { center_x, center_y };

// ���� ��� (���� �� �� ���ϰ� �ϱ�)
const double RESTITUTION = 0.3f;      // ź�� ���
const double dragForce = 150.0f;      // ���� ��
const double maxForce = 200.0f;      // �ִ� ���� ��

// ������ ����
float line_length = 0.15f;

void start_game(char* username);