#pragma once
#include <stdbool.h>
#include "vector.h"
#include "planet.h"

// 물리 계수 (수정 시 꼭 말하고 하기)
extern const double GRAVITY;  // 중력
extern const double centerCoefficient;	// 회전 보정

// 충돌 체크 함수
bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2);
// 핵심 중력 + 회전 보정 함수
void CalcGravity(Planet* rb, Vector2 center, float deltaTime);