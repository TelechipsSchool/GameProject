#pragma once
#include <stdbool.h>
#include "vector.h"
#include "planet.h"

// ���� ��� (���� �� �� ���ϰ� �ϱ�)
const double GRAVITY = 9.8f;  // �߷�

// �浹 üũ �Լ�
bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2);
// �ٽ� �߷� + ȸ�� ���� �Լ�
void CalcGravity(Planet* rb, Vector2 center, float centerCoefficient, float deltaTime);