#include "physics.h"

// 충돌 체크 함수
bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < size1 + size2;
}

// 핵심 중력 + 회전 보정 함수
void CalcGravity(Planet* rb, Vector2 center, float centerCoefficient, float deltaTime) {
    Vector2 gravityDir = Vector2_Normalize(Vector2_Sub(center, rb->position));
    Vector2 gravity = Vector2_Scale(gravityDir, 9.8f);

    Vector2 vel = rb->velocity;
    Vector2 nextPos = Vector2_Add(rb->position, vel);

    Vector2 parallelVel = Vector2_Project(vel, gravityDir);
    Vector2 perpendicularVel = Vector2_Sub(vel, parallelVel);

    Vector2 correctionDir = Vector2_Project((Vector2) { -perpendicularVel.x, -perpendicularVel.y }, Vector2_Sub(nextPos, center));
    Vector2 newPerpendicularVel = Vector2_Add(perpendicularVel, correctionDir);

    perpendicularVel.x = (1 - centerCoefficient) * perpendicularVel.x + centerCoefficient * newPerpendicularVel.x;
    perpendicularVel.y = (1 - centerCoefficient) * perpendicularVel.y + centerCoefficient * newPerpendicularVel.y;

    rb->velocity = Vector2_Add(Vector2_Add(parallelVel, perpendicularVel), Vector2_Scale(gravity, deltaTime));
}