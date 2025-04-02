#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "font.h"

#define RANKING_FILE "ranking.txt"
#define MAX_RANKS 10

typedef struct {
    char username[100];  // 사용자 이름
    int score;           // 점수
} Rank;

int load_score(Rank* ranks);
int get_high_score();
void save_score(const char* username, int score);
void display_ranks();
//ALLEGRO_FONT* get_rank_font();
int compare_ranks(const void* a, const void* b);