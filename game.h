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
float gravity_strength  = 0.05;  // �߷�
float restitution = 1;      // ź�� ���

typedef struct {
    float x, y;
    float speed;
    float angle;
    int width;
    int height;
    ALLEGRO_BITMAP* image;
} Planet;

void start_game();
Planet create_planet(float x, float y, float speed, float angle, int width, int height, const char* image_path);
bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2);
// �ٽ� �߷� + ȸ�� ���� �Լ�
void CalcGravity(Rigidbody2D* rb, Vector2 center, float gravityScale, float centerCoefficient, float deltaTime);