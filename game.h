#pragma once
#include "init.h"
#include "display.h"
#include "vector.h"

// ������ ����
#define line_length 100

// �༺ �߻� ��ġ
#define init_x 300
#define init_y SCREEN_H / 2

// ��ǥ �༺ ��ġ
#define center_x SCREEN_W - 400
#define center_y SCREEN_H / 2

// ���� ���
float GRAVITY = 9.8f;  // �߷�
float restitution = 1;      // ź�� ���

void start_game();
bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2);
// �ٽ� �߷� + ȸ�� ���� �Լ�
void CalcGravity(Rigidbody2D* rb, Vector2 center, float centerCoefficient, float deltaTime);