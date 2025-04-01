#include "rank.h"

// 랭킹을 파일에서 읽어오는 함수
int load_score(Rank* ranks) {
    FILE* file = fopen(RANKING_FILE, "r");

    int rank_count = 0;

    if (file) {
        while (fscanf(file, "%s %d", ranks[rank_count].username, &ranks[rank_count].score) == 2) {
            rank_count++;
            if (rank_count >= MAX_RANKS) break;  // 최대 랭킹 개수 제한
        }
        fclose(file);
    }
    else {
        printf("fail to load ranks\n");
    }
    return rank_count;  // 불러온 랭킹의 개수 반환
}

int get_high_score() {
    Rank ranks[MAX_RANKS];  // 랭킹 저장 배열
    int rank_count = load_score(ranks);  // 랭킹 로드

    if (rank_count == 0) {
        return 0;  // 랭킹이 없으면 0 반환
    }

    int max_score = 0;  // 첫 번째 점수를 초기 최대값으로 설정

    for (int i = 1; i < rank_count; i++) {
        if (ranks[i].score > max_score) {
            max_score = ranks[i].score;
        }
    }

    return max_score;
}


// 새로운 점수를 파일에 저장하는 함수
void save_score(const char* username, int score) {
    FILE* file = fopen(RANKING_FILE, "a");
    if (file) {
        fprintf(file, "%s %d\n", username, score);
        fclose(file);
    }
}

void display_ranks() {
    ALLEGRO_FONT* rank_font = get_rank_font();

    Rank ranks[MAX_RANKS];

    int rank_count = load_score(ranks);

    bool running = true;

    al_clear_to_color(al_map_rgb(20, 20, 20));

    while (running) {
        int y = 200;

        // 이벤트 큐 생성
        ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
        al_register_event_source(event_queue, al_get_keyboard_event_source());
        ALLEGRO_EVENT rank_event;
        al_wait_for_event(event_queue, &rank_event);

        if (rank_event.type == ALLEGRO_EVENT_KEY_DOWN &&
            rank_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            running = false; // ESC 누르면 루프 종료
            menu();
        }

        al_draw_text(rank_font, al_map_rgb(255, 255, 0),
            SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "RANKING");

        // 화면에 텍스트를 출력
        for (int i = 0; i < rank_count && i < MAX_RANKS; ++i) {
            // 이름 - 점수 표시
            al_draw_textf(rank_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y, ALLEGRO_ALIGN_CENTER, "%d. %s - %d", i + 1, ranks[i].username, ranks[i].score);
            y += 50;  // 각 랭킹 사이 간격을 두기 위해 y 좌표 증가
        }

        // 화면 업데이트
        al_flip_display();
    }
}