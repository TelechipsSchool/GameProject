#pragma once
#include <stdbool.h>
#include "vector.h"
#include "planet.h"

// ���� ��� (���� �� �� ���ϰ� �ϱ�)
extern const double GRAVITY;  // �߷�
extern const double centerCoefficient;	// ȸ�� ����

// �浹 üũ �Լ�
bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2);
// �ٽ� �߷� + ȸ�� ���� �Լ�
void CalcGravity(Planet* rb, Vector2 center, float deltaTime);