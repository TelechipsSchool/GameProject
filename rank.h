#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "font.h"
#include "audio.h"

#define RANKING_FILE "ranking.txt"
#define MAX_RANKS 10

typedef struct {
    char username[100];  // ����� �̸�
    int score;           // ����
} Rank;

int load_score(Rank* ranks);
int get_high_score();
void save_score(const char* username, int score);
void display_ranks();
int compare_ranks(const void* a, const void* b);