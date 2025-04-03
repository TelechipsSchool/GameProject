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

// 유도선 설정
float line_length = 0.15f;

// 행성 발사 위치
#define init_x 300
#define init_y SCREEN_H / 2


//배경 전용 타이머
float background_timer = 0.0f;
float background_interval = 0.3f;

// 행성 대기 위치
#define wait_x 120
#define wait_y 120

// 목표 행성 위치
#define center_x SCREEN_W - 400
#define center_y SCREEN_H / 2

// 물리 계수 (수정 시 꼭 말하고 하기)
double centerCoefficient = 0.03f;
double RESTITUTION = 0.3f;      // 탄성 계수
double dragForce = 150.0f;      // 당기는 힘
double maxForce = 200.0f;      // 최대 당기는 힘

void start_game(char* username);