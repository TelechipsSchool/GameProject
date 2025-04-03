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

//배경 전용 타이머
float background_timer = 0.0f;
const float background_interval = 0.3f;

// 행성 발사 위치
#define init_x 300
#define init_y SCREEN_H / 2

// 행성 대기 위치
#define wait_x 120
#define wait_y 120

// 발사 지점
Vector2 shootStart = { init_x, init_y };
Vector2 waitPoint = { wait_x, wait_y };

// 목표 행성 위치
#define center_x SCREEN_W - 400
#define center_y SCREEN_H / 2
const Vector2 gravityCenter = { center_x, center_y };

// 물리 계수 (수정 시 꼭 말하고 하기)
const double RESTITUTION = 0.3f;      // 탄성 계수
const double dragForce = 150.0f;      // 당기는 힘
const double maxForce = 200.0f;      // 최대 당기는 힘

// 유도선 설정
float line_length = 0.15f;

void start_game(char* username);